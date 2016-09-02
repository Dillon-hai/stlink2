/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stlink2.h>
#include <stlink2/utils/hexstr.h>

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define STLINK2_USB_RX_EP      (1 | LIBUSB_ENDPOINT_IN)  /**< USB RX endpoint */
#define STLINK2_USB_V2_TX_EP   (2 | LIBUSB_ENDPOINT_OUT) /**< USB TX endpoint for Stlink2 */
#define STLINK2_USB_V2_1_TX_EP (1 | LIBUSB_ENDPOINT_OUT) /**< USB TX endpoint for Stlink2-1 */

/** @todo move to aux */
static char *stlink2_strdup(const char *s)
{
	const size_t n = strlen(s);
	char *p = malloc(n + 1);

	if (p) {
		memcpy(p, s, n);
		p[n] = 0;
	}

	return p;
}

static bool stlink2_usb_claim(struct stlink2 *dev)
{
	int ret;
	int config;

	ret = libusb_kernel_driver_active(dev->usb.dev, 0);
	if (ret) {
		ret = libusb_detach_kernel_driver(dev->usb.dev, 0);
		if (ret == LIBUSB_ERROR_NOT_SUPPORTED) {
			STLINK2_LOG(TRACE, dev, "libusb_detach_kernel_driver (%s)\n", libusb_error_name(ret));
		} else if (ret) {
			STLINK2_LOG(ERROR, dev, "libusb_detach_kernel_driver failed (%s)\n", libusb_error_name(ret));
			return false;
		}
	}

	ret = libusb_get_configuration(dev->usb.dev, &config);
	if (ret) {
		STLINK2_LOG(ERROR, dev, "libusb_get_configuration failed (%s)\n", libusb_error_name(ret));
		return false;
	}

	ret = libusb_set_configuration(dev->usb.dev, 1);
	if (ret) {
		STLINK2_LOG(ERROR, dev, "libusb_set_configuration failed (%s)\n", libusb_error_name(ret));
		return false;
	}

	ret = libusb_claim_interface(dev->usb.dev, 0);
	if (ret) {
		STLINK2_LOG(ERROR, dev, "libusb_claim_interface failed (%s)\n", libusb_error_name(ret));
		return false;
	}

	return true;
}

static char *stlink2_usb_read_binary_serial(const char *serial, size_t len)
{
	/** @todo need to get rid of this weird calculation... + 1, - 1 */
	const size_t size = (len * 2) + 1;
	char *_serial = malloc(size);

	if (!_serial)
		return NULL;

	stlink2_hexstr_from_bin(_serial, size - 1, serial, len);
	_serial[size - 1] = 0;

	return _serial;
}

/**
 * Read binary or hex encoded serial from usb handle
 * @note The pointer must be freed by the callee when != NULL
 * @return hex encoded string
 */
static char *stlink2_usb_read_serial(struct stlink2 *st, libusb_device_handle *handle,
				     struct libusb_device_descriptor *desc)
{
	int ret;
	char serial[8192];
	bool ishexserial = true;

	memset(serial, 0, sizeof(serial));

	ret = libusb_get_string_descriptor_ascii(handle, desc->iSerialNumber, (unsigned char *)&serial, sizeof(serial));
	if (ret < 0) {
		STLINK2_LOG(ERROR, st, "libusb_get_string_descriptor_ascii failed (%s)\n", libusb_error_name(ret));
		return NULL;
	}

	for (int n = 0; n < ret; n++) {
		if (isxdigit(serial[n]))
			continue;

		ishexserial = false;
		break;
	}

	if (!ishexserial)
		return stlink2_usb_read_binary_serial(serial, ret);

	return stlink2_strdup(serial);
}

bool stlink2_usb_probe_dev(libusb_device *dev, struct stlink2 *st)
{
	int ret = 0;
	struct libusb_device_descriptor desc;
	libusb_device_handle *devh;

	ret = libusb_get_device_descriptor(dev, &desc);
	if (ret) {
		STLINK2_LOG(ERROR, st, "libusb_get_device_descriptor failed (%s)\n", libusb_error_name(ret));
		return false;
	}

	if (desc.idProduct != STLINK2_USB_PID_V2 &&
	    desc.idProduct != STLINK2_USB_PID_V2_1)
		return false;

	ret = libusb_open(dev, &devh);
	if (ret) {
		STLINK2_LOG(ERROR, st, "libusb_open failed (%s)\n", libusb_error_name(ret));
		return false;
	}

	stlink2_log_set_file(st, stdout);
	stlink2_log_set_level(st, STLINK2_LOGLEVEL_INFO);

	st->usb.timeout = 3000;
	st->usb.dev     = devh;
	st->usb.pid     = desc.idProduct;

	st->serial = stlink2_usb_read_serial(st, st->usb.dev, &desc);
	if (!st->serial) {
		STLINK2_LOG(ERROR, st, "stlink2_usb_read_serial failed\n");
		libusb_close(devh);
		return false;
	}

	stlink2_usb_claim(st);
	stlink2_usb_config_endpoints(st);
	stlink2_usb_set_name_from_pid(st);

	return true;
}

/**
 * Set programmer name based on USB PID
 */
void stlink2_usb_set_name_from_pid(struct stlink2 *dev)
{
	static const char *stlinkv2   = "st-link/v2";
	static const char *stlinkv2_1 = "st-link/v2-1";

	if (dev->usb.pid == STLINK2_USB_PID_V2)
		dev->name = stlinkv2;
	else if (dev->usb.pid == STLINK2_USB_PID_V2_1)
		dev->name = stlinkv2_1;
}

void stlink2_usb_config_endpoints(struct stlink2 *dev)
{
	dev->usb.tx_ep = STLINK2_USB_RX_EP;
	if (dev->usb.pid == STLINK2_USB_PID_V2)
		dev->usb.rx_ep = STLINK2_USB_V2_TX_EP;
	else if (dev->usb.pid == STLINK2_USB_PID_V2_1)
		dev->usb.rx_ep = STLINK2_USB_V2_1_TX_EP;
}

ssize_t stlink2_usb_send_recv(struct stlink2 *dev,
			      uint8_t *txbuf, size_t txsize,
			      uint8_t *rxbuf, size_t rxsize)
{
	int ret;
	int res;

	ret = libusb_bulk_transfer(dev->usb.dev, dev->usb.rx_ep,
				   txbuf,
				   (int)txsize,
				   &res,
				   dev->usb.timeout);
	if (ret) {
		STLINK2_LOG(ERROR, dev, "libusb_bulk_transfer tx failed (%s)\n", libusb_error_name(ret));
		return 0;
	}

	STLINK2_LOG(TRACE, dev, "USB > ");
	for (size_t n = 0; n < txsize; n++)
		STLINK2_LOG_WRITE(TRACE, dev, "%02x ", txbuf[n]);
	STLINK2_LOG_WRITE(TRACE, dev, "\n");

	if (!rxbuf || !rxsize)
		return 0;

	ret = libusb_bulk_transfer(dev->usb.dev, dev->usb.tx_ep,
				   rxbuf,
				   (int)rxsize,
				   &res,
				   dev->usb.timeout);
	if (ret) {
		STLINK2_LOG(ERROR, dev, "libusb_bulk_transfer rx failed (%s)\n", libusb_error_name(ret));
		return 0;
	}

	STLINK2_LOG(TRACE, dev, "USB < ");
	for (size_t n = 0; n < rxsize; n++)
		STLINK2_LOG_WRITE(TRACE, dev, "%02x ", rxbuf[n]);
	STLINK2_LOG_WRITE(TRACE, dev, "\n");

	return 0;
}
