// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 Bootlin
 * Copyright (c) 2022 Maíra Canal <mairacanal@riseup.net>
 */

#include <kunit/test.h>

#include <drm/drm_connector.h>
#include <drm/drm_modes.h>

static const struct drm_connector no_connector = {};

static void drm_cmdline_test_force_e_only(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "e";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_FALSE(test, mode.specified);
	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);
	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_ON);
}

static void drm_cmdline_test_force_D_only_not_digital(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "D";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_FALSE(test, mode.specified);
	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);
	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_ON);
}

static const struct drm_connector connector_hdmi = {
	.connector_type	= DRM_MODE_CONNECTOR_HDMIB,
};

static void drm_cmdline_test_force_D_only_hdmi(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "D";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &connector_hdmi, &mode));
	KUNIT_EXPECT_FALSE(test, mode.specified);
	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);
	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_ON_DIGITAL);
}

static const struct drm_connector connector_dvi = {
	.connector_type	= DRM_MODE_CONNECTOR_DVII,
};

static void drm_cmdline_test_force_D_only_dvi(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "D";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &connector_dvi, &mode));
	KUNIT_EXPECT_FALSE(test, mode.specified);
	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);
	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_ON_DIGITAL);
}

static void drm_cmdline_test_force_d_only(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "d";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_FALSE(test, mode.specified);
	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);
	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_OFF);
}

static void drm_cmdline_test_margin_only(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "m";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_interlace_only(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "i";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_res(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_res_missing_x(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "x480";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_res_missing_y(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "1024x";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_res_bad_y(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "1024xtest";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_res_missing_y_bpp(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "1024x-24";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_res_vesa(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480M";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_TRUE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_res_vesa_rblank(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480MR";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_TRUE(test, mode.rb);
	KUNIT_EXPECT_TRUE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_res_rblank(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480R";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_TRUE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_res_bpp(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480-24";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_TRUE(test, mode.bpp_specified);
	KUNIT_EXPECT_EQ(test, mode.bpp, 24);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_res_bad_bpp(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480-test";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_res_refresh(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480@60";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);

	KUNIT_EXPECT_TRUE(test, mode.refresh_specified);
	KUNIT_EXPECT_EQ(test, mode.refresh, 60);

	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_res_bad_refresh(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480@refresh";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_res_bpp_refresh(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480-24@60";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);

	KUNIT_EXPECT_TRUE(test, mode.refresh_specified);
	KUNIT_EXPECT_EQ(test, mode.refresh, 60);

	KUNIT_EXPECT_TRUE(test, mode.bpp_specified);
	KUNIT_EXPECT_EQ(test, mode.bpp, 24);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_res_bpp_refresh_interlaced(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480-24@60i";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);

	KUNIT_EXPECT_TRUE(test, mode.refresh_specified);
	KUNIT_EXPECT_EQ(test, mode.refresh, 60);

	KUNIT_EXPECT_TRUE(test, mode.bpp_specified);
	KUNIT_EXPECT_EQ(test, mode.bpp, 24);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_TRUE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_res_bpp_refresh_margins(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline =  "720x480-24@60m";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);

	KUNIT_EXPECT_TRUE(test, mode.refresh_specified);
	KUNIT_EXPECT_EQ(test, mode.refresh, 60);

	KUNIT_EXPECT_TRUE(test, mode.bpp_specified);
	KUNIT_EXPECT_EQ(test, mode.bpp, 24);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_TRUE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_res_bpp_refresh_force_off(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline =  "720x480-24@60d";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);

	KUNIT_EXPECT_TRUE(test, mode.refresh_specified);
	KUNIT_EXPECT_EQ(test, mode.refresh, 60);

	KUNIT_EXPECT_TRUE(test, mode.bpp_specified);
	KUNIT_EXPECT_EQ(test, mode.bpp, 24);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_OFF);
}

static void drm_cmdline_test_res_bpp_refresh_force_on_off(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline =  "720x480-24@60de";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_res_bpp_refresh_force_on(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline =  "720x480-24@60e";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);

	KUNIT_EXPECT_TRUE(test, mode.refresh_specified);
	KUNIT_EXPECT_EQ(test, mode.refresh, 60);

	KUNIT_EXPECT_TRUE(test, mode.bpp_specified);
	KUNIT_EXPECT_EQ(test, mode.bpp, 24);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_ON);
}

static void drm_cmdline_test_res_bpp_refresh_force_on_analog(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480-24@60D";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);

	KUNIT_EXPECT_TRUE(test, mode.refresh_specified);
	KUNIT_EXPECT_EQ(test, mode.refresh, 60);

	KUNIT_EXPECT_TRUE(test, mode.bpp_specified);
	KUNIT_EXPECT_EQ(test, mode.bpp, 24);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_ON);
}

static void drm_cmdline_test_res_bpp_refresh_force_on_digital(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	static const struct drm_connector connector = {
		.connector_type = DRM_MODE_CONNECTOR_DVII,
	};
	const char *cmdline = "720x480-24@60D";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);

	KUNIT_EXPECT_TRUE(test, mode.refresh_specified);
	KUNIT_EXPECT_EQ(test, mode.refresh, 60);

	KUNIT_EXPECT_TRUE(test, mode.bpp_specified);
	KUNIT_EXPECT_EQ(test, mode.bpp, 24);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_ON_DIGITAL);
}

static void drm_cmdline_test_res_bpp_refresh_interlaced_margins_force_on(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480-24@60ime";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);

	KUNIT_EXPECT_TRUE(test, mode.refresh_specified);
	KUNIT_EXPECT_EQ(test, mode.refresh, 60);

	KUNIT_EXPECT_TRUE(test, mode.bpp_specified);
	KUNIT_EXPECT_EQ(test, mode.bpp, 24);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_TRUE(test, mode.interlace);
	KUNIT_EXPECT_TRUE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_ON);
}

static void drm_cmdline_test_res_margins_force_on(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480me";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_TRUE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_ON);
}

static void drm_cmdline_test_res_vesa_margins(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480Mm";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_TRUE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_TRUE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_res_invalid_mode(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480f";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_res_bpp_wrong_place_mode(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480e-24";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_name(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "NTSC";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_STREQ(test, mode.name, "NTSC");
	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);
	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);
}

static void drm_cmdline_test_name_bpp(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "NTSC-24";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_STREQ(test, mode.name, "NTSC");

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_TRUE(test, mode.bpp_specified);
	KUNIT_EXPECT_EQ(test, mode.bpp, 24);
}

static void drm_cmdline_test_name_bpp_refresh(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "NTSC-24@60";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_name_refresh(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "NTSC@60";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_name_refresh_wrong_mode(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "NTSC@60m";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_name_refresh_invalid_mode(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "NTSC@60f";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_name_option(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "NTSC,rotate=180";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_STREQ(test, mode.name, "NTSC");
	KUNIT_EXPECT_EQ(test, mode.rotation_reflection, DRM_MODE_ROTATE_180);
}

static void drm_cmdline_test_name_bpp_option(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "NTSC-24,rotate=180";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_STREQ(test, mode.name, "NTSC");
	KUNIT_EXPECT_EQ(test, mode.rotation_reflection, DRM_MODE_ROTATE_180);
	KUNIT_EXPECT_TRUE(test, mode.bpp_specified);
	KUNIT_EXPECT_EQ(test, mode.bpp, 24);
}

static void drm_cmdline_test_rotate_0(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480,rotate=0";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);
	KUNIT_EXPECT_EQ(test, mode.rotation_reflection, DRM_MODE_ROTATE_0);

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_rotate_90(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480,rotate=90";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);
	KUNIT_EXPECT_EQ(test, mode.rotation_reflection, DRM_MODE_ROTATE_90);

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_rotate_180(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480,rotate=180";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);
	KUNIT_EXPECT_EQ(test, mode.rotation_reflection, DRM_MODE_ROTATE_180);

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_rotate_270(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480,rotate=270";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);
	KUNIT_EXPECT_EQ(test, mode.rotation_reflection, DRM_MODE_ROTATE_270);

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_rotate_multiple(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480,rotate=0,rotate=90";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_rotate_invalid_val(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480,rotate=42";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_rotate_truncated(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480,rotate=";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_hmirror(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480,reflect_x";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);
	KUNIT_EXPECT_EQ(test, mode.rotation_reflection, (DRM_MODE_ROTATE_0 | DRM_MODE_REFLECT_X));

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_vmirror(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480,reflect_y";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);
	KUNIT_EXPECT_EQ(test, mode.rotation_reflection, (DRM_MODE_ROTATE_0 | DRM_MODE_REFLECT_Y));

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_margin_options(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline =
		"720x480,margin_right=14,margin_left=24,margin_bottom=36,margin_top=42";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);
	KUNIT_EXPECT_EQ(test, mode.tv_margins.right, 14);
	KUNIT_EXPECT_EQ(test, mode.tv_margins.left, 24);
	KUNIT_EXPECT_EQ(test, mode.tv_margins.bottom, 36);
	KUNIT_EXPECT_EQ(test, mode.tv_margins.top, 42);

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_multiple_options(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480,rotate=270,reflect_x";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);
	KUNIT_EXPECT_EQ(test, mode.rotation_reflection, (DRM_MODE_ROTATE_270 | DRM_MODE_REFLECT_X));

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_tv_options(struct kunit *test,
					const char *cmdline,
					const struct drm_display_mode *expected_mode,
					unsigned int expected_tv_mode)
{
	struct drm_cmdline_mode mode = { };

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, expected_mode->hdisplay);
	KUNIT_EXPECT_EQ(test, mode.yres, expected_mode->vdisplay);
	KUNIT_EXPECT_EQ(test, mode.tv_mode, expected_tv_mode);

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_EQ(test, mode.interlace, !!(expected_mode->flags & DRM_MODE_FLAG_INTERLACE));
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_tv_option_hd480i(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x480i,tv_mode=HD480I",
				    &drm_mode_480i,
				    DRM_MODE_TV_NORM_HD480I);
}

static const struct drm_display_mode drm_mode_480p = {
	DRM_MODE("720x480", DRM_MODE_TYPE_DRIVER, 27000,
		 720, 735, 743, 858, 0, 480, 490, 494, 525, 0,
		 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC)
};

static void drm_cmdline_test_tv_option_hd480p(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x480,tv_mode=HD480P",
				    &drm_mode_480p,
				    DRM_MODE_TV_NORM_HD480P);
}

static void drm_cmdline_test_tv_option_hd576i(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576i,tv_mode=HD576I",
				    &drm_mode_576i,
				    DRM_MODE_TV_NORM_HD576I);
}

static const struct drm_display_mode drm_mode_576p = {
	DRM_MODE("720x576", DRM_MODE_TYPE_DRIVER, 27000,
		 720, 730, 738, 864, 0, 576, 581, 585, 625, 0,
		 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC)
};

static void drm_cmdline_test_tv_option_hd576p(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576,tv_mode=HD576P",
				    &drm_mode_576p,
				    DRM_MODE_TV_NORM_HD576P);
}

static const struct drm_display_mode drm_mode_720p = {
	DRM_MODE("1280x720", DRM_MODE_TYPE_DRIVER, 74250,
		 1280, 1349, 1357, 1650, 0, 720, 725, 730, 750, 0,
		 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC)
};

static void drm_cmdline_test_tv_option_hd720p(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "1280x720,tv_mode=HD720P",
				    &drm_mode_720p,
				    DRM_MODE_TV_NORM_HD720P);
}

static const struct drm_display_mode drm_mode_1080i = {
	DRM_MODE("1920x1080", DRM_MODE_TYPE_DRIVER, 74250,
		 1920, 1961, 2049, 2200, 0, 1080, 1084, 1088, 1125, 0,
		 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC
		 | DRM_MODE_FLAG_INTERLACE)
};

static void drm_cmdline_test_tv_option_hd1080i(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "1920x1080i,tv_mode=HD1080I",
				    &drm_mode_1080i,
				    DRM_MODE_TV_NORM_HD1080I);
}

static void drm_cmdline_test_tv_option_ntsc_443(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x480i,tv_mode=NTSC-443",
				    &drm_mode_480i,
				    DRM_MODE_TV_NORM_NTSC_443);
}

static void drm_cmdline_test_tv_option_ntsc_j(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x480i,tv_mode=NTSC-J",
				    &drm_mode_480i,
				    DRM_MODE_TV_NORM_NTSC_J);
}

static void drm_cmdline_test_tv_option_ntsc_m(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x480i,tv_mode=NTSC-M",
				    &drm_mode_480i,
				    DRM_MODE_TV_NORM_NTSC_M);
}

static void drm_cmdline_test_tv_option_pal_60(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576i,tv_mode=PAL-60",
				    &drm_mode_576i,
				    DRM_MODE_TV_NORM_PAL_60);
}

static void drm_cmdline_test_tv_option_pal_b(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576i,tv_mode=PAL-B",
				    &drm_mode_576i,
				    DRM_MODE_TV_NORM_PAL_B);
}

static void drm_cmdline_test_tv_option_pal_d(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576i,tv_mode=PAL-D",
				    &drm_mode_576i,
				    DRM_MODE_TV_NORM_PAL_D);
}

static void drm_cmdline_test_tv_option_pal_g(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576i,tv_mode=PAL-G",
				    &drm_mode_576i,
				    DRM_MODE_TV_NORM_PAL_G);
}

static void drm_cmdline_test_tv_option_pal_h(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576i,tv_mode=PAL-H",
				    &drm_mode_576i,
				    DRM_MODE_TV_NORM_PAL_H);
}

static void drm_cmdline_test_tv_option_pal_i(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576i,tv_mode=PAL-I",
				    &drm_mode_576i,
				    DRM_MODE_TV_NORM_PAL_I);
}

static void drm_cmdline_test_tv_option_pal_m(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x480i,tv_mode=PAL-M",
				    &drm_mode_480i,
				    DRM_MODE_TV_NORM_PAL_M);
}

static void drm_cmdline_test_tv_option_pal_n(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576i,tv_mode=PAL-N",
				    &drm_mode_576i,
				    DRM_MODE_TV_NORM_PAL_N);
}

static void drm_cmdline_test_tv_option_pal_nc(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576i,tv_mode=PAL-NC",
				    &drm_mode_576i,
				    DRM_MODE_TV_NORM_PAL_NC);
}

static void drm_cmdline_test_tv_option_secam_60(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576i,tv_mode=SECAM-60",
				    &drm_mode_576i,
				    DRM_MODE_TV_NORM_SECAM_60);
}

static void drm_cmdline_test_tv_option_secam_b(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576i,tv_mode=SECAM-B",
				    &drm_mode_576i,
				    DRM_MODE_TV_NORM_SECAM_B);
}

static void drm_cmdline_test_tv_option_secam_d(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576i,tv_mode=SECAM-D",
				    &drm_mode_576i,
				    DRM_MODE_TV_NORM_SECAM_D);
}

static void drm_cmdline_test_tv_option_secam_g(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576i,tv_mode=SECAM-G",
				    &drm_mode_576i,
				    DRM_MODE_TV_NORM_SECAM_G);
}

static void drm_cmdline_test_tv_option_secam_k(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576i,tv_mode=SECAM-K",
				    &drm_mode_576i,
				    DRM_MODE_TV_NORM_SECAM_K);
}

static void drm_cmdline_test_tv_option_secam_k1(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576i,tv_mode=SECAM-K1",
				    &drm_mode_576i,
				    DRM_MODE_TV_NORM_SECAM_K1);
}

static void drm_cmdline_test_tv_option_secam_l(struct kunit *test)
{
	drm_cmdline_test_tv_options(test,
				    "720x576i,tv_mode=SECAM-L",
				    &drm_mode_576i,
				    DRM_MODE_TV_NORM_SECAM_L);
}

static void drm_cmdline_test_tv_option_invalid(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480i,tv_mode=invalid";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_tv_option_truncated(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480i,tv_mode=NTSC";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_invalid_option(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480,test=42";

	KUNIT_EXPECT_FALSE(test, drm_mode_parse_command_line_for_connector(cmdline,
									   &no_connector, &mode));
}

static void drm_cmdline_test_bpp_extra_and_option(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480-24e,rotate=180";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);
	KUNIT_EXPECT_EQ(test, mode.rotation_reflection, DRM_MODE_ROTATE_180);

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);

	KUNIT_EXPECT_TRUE(test, mode.bpp_specified);
	KUNIT_EXPECT_EQ(test, mode.bpp, 24);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_ON);
}

static void drm_cmdline_test_extra_and_option(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "720x480e,rotate=180";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_TRUE(test, mode.specified);
	KUNIT_EXPECT_EQ(test, mode.xres, 720);
	KUNIT_EXPECT_EQ(test, mode.yres, 480);
	KUNIT_EXPECT_EQ(test, mode.rotation_reflection, DRM_MODE_ROTATE_180);

	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);
	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_ON);
}

static void drm_cmdline_test_freestanding_options(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "margin_right=14,margin_left=24,margin_bottom=36,margin_top=42";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_FALSE(test, mode.specified);
	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);
	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_EQ(test, mode.tv_margins.right, 14);
	KUNIT_EXPECT_EQ(test, mode.tv_margins.left, 24);
	KUNIT_EXPECT_EQ(test, mode.tv_margins.bottom, 36);
	KUNIT_EXPECT_EQ(test, mode.tv_margins.top, 42);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static void drm_cmdline_test_freestanding_force_e_and_options(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "e,margin_right=14,margin_left=24,margin_bottom=36,margin_top=42";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_FALSE(test, mode.specified);
	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);
	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_EQ(test, mode.tv_margins.right, 14);
	KUNIT_EXPECT_EQ(test, mode.tv_margins.left, 24);
	KUNIT_EXPECT_EQ(test, mode.tv_margins.bottom, 36);
	KUNIT_EXPECT_EQ(test, mode.tv_margins.top, 42);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_ON);
}

static void drm_cmdline_test_panel_orientation(struct kunit *test)
{
	struct drm_cmdline_mode mode = { };
	const char *cmdline = "panel_orientation=upside_down";

	KUNIT_EXPECT_TRUE(test, drm_mode_parse_command_line_for_connector(cmdline,
									  &no_connector, &mode));
	KUNIT_EXPECT_FALSE(test, mode.specified);
	KUNIT_EXPECT_FALSE(test, mode.refresh_specified);
	KUNIT_EXPECT_FALSE(test, mode.bpp_specified);

	KUNIT_EXPECT_EQ(test, mode.panel_orientation, DRM_MODE_PANEL_ORIENTATION_BOTTOM_UP);

	KUNIT_EXPECT_FALSE(test, mode.rb);
	KUNIT_EXPECT_FALSE(test, mode.cvt);
	KUNIT_EXPECT_FALSE(test, mode.interlace);
	KUNIT_EXPECT_FALSE(test, mode.margins);
	KUNIT_EXPECT_EQ(test, mode.force, DRM_FORCE_UNSPECIFIED);
}

static struct kunit_case drm_cmdline_parser_tests[] = {
	KUNIT_CASE(drm_cmdline_test_force_d_only),
	KUNIT_CASE(drm_cmdline_test_force_D_only_dvi),
	KUNIT_CASE(drm_cmdline_test_force_D_only_hdmi),
	KUNIT_CASE(drm_cmdline_test_force_D_only_not_digital),
	KUNIT_CASE(drm_cmdline_test_force_e_only),
	KUNIT_CASE(drm_cmdline_test_margin_only),
	KUNIT_CASE(drm_cmdline_test_interlace_only),
	KUNIT_CASE(drm_cmdline_test_res),
	KUNIT_CASE(drm_cmdline_test_res_missing_x),
	KUNIT_CASE(drm_cmdline_test_res_missing_y),
	KUNIT_CASE(drm_cmdline_test_res_bad_y),
	KUNIT_CASE(drm_cmdline_test_res_missing_y_bpp),
	KUNIT_CASE(drm_cmdline_test_res_vesa),
	KUNIT_CASE(drm_cmdline_test_res_vesa_rblank),
	KUNIT_CASE(drm_cmdline_test_res_rblank),
	KUNIT_CASE(drm_cmdline_test_res_bpp),
	KUNIT_CASE(drm_cmdline_test_res_bad_bpp),
	KUNIT_CASE(drm_cmdline_test_res_refresh),
	KUNIT_CASE(drm_cmdline_test_res_bad_refresh),
	KUNIT_CASE(drm_cmdline_test_res_bpp_refresh),
	KUNIT_CASE(drm_cmdline_test_res_bpp_refresh_interlaced),
	KUNIT_CASE(drm_cmdline_test_res_bpp_refresh_margins),
	KUNIT_CASE(drm_cmdline_test_res_bpp_refresh_force_off),
	KUNIT_CASE(drm_cmdline_test_res_bpp_refresh_force_on_off),
	KUNIT_CASE(drm_cmdline_test_res_bpp_refresh_force_on),
	KUNIT_CASE(drm_cmdline_test_res_bpp_refresh_force_on_analog),
	KUNIT_CASE(drm_cmdline_test_res_bpp_refresh_force_on_digital),
	KUNIT_CASE(drm_cmdline_test_res_bpp_refresh_interlaced_margins_force_on),
	KUNIT_CASE(drm_cmdline_test_res_margins_force_on),
	KUNIT_CASE(drm_cmdline_test_res_vesa_margins),
	KUNIT_CASE(drm_cmdline_test_res_invalid_mode),
	KUNIT_CASE(drm_cmdline_test_res_bpp_wrong_place_mode),
	KUNIT_CASE(drm_cmdline_test_name),
	KUNIT_CASE(drm_cmdline_test_name_bpp),
	KUNIT_CASE(drm_cmdline_test_name_refresh),
	KUNIT_CASE(drm_cmdline_test_name_bpp_refresh),
	KUNIT_CASE(drm_cmdline_test_name_refresh_wrong_mode),
	KUNIT_CASE(drm_cmdline_test_name_refresh_invalid_mode),
	KUNIT_CASE(drm_cmdline_test_name_option),
	KUNIT_CASE(drm_cmdline_test_name_bpp_option),
	KUNIT_CASE(drm_cmdline_test_tv_option_hd480i),
	KUNIT_CASE(drm_cmdline_test_tv_option_hd480p),
	KUNIT_CASE(drm_cmdline_test_tv_option_hd576i),
	KUNIT_CASE(drm_cmdline_test_tv_option_hd576p),
	KUNIT_CASE(drm_cmdline_test_tv_option_hd720p),
	KUNIT_CASE(drm_cmdline_test_tv_option_hd1080i),
	KUNIT_CASE(drm_cmdline_test_tv_option_ntsc_443),
	KUNIT_CASE(drm_cmdline_test_tv_option_ntsc_j),
	KUNIT_CASE(drm_cmdline_test_tv_option_ntsc_m),
	KUNIT_CASE(drm_cmdline_test_tv_option_pal_60),
	KUNIT_CASE(drm_cmdline_test_tv_option_pal_b),
	KUNIT_CASE(drm_cmdline_test_tv_option_pal_d),
	KUNIT_CASE(drm_cmdline_test_tv_option_pal_g),
	KUNIT_CASE(drm_cmdline_test_tv_option_pal_h),
	KUNIT_CASE(drm_cmdline_test_tv_option_pal_i),
	KUNIT_CASE(drm_cmdline_test_tv_option_pal_m),
	KUNIT_CASE(drm_cmdline_test_tv_option_pal_n),
	KUNIT_CASE(drm_cmdline_test_tv_option_pal_nc),
	KUNIT_CASE(drm_cmdline_test_tv_option_secam_60),
	KUNIT_CASE(drm_cmdline_test_tv_option_secam_b),
	KUNIT_CASE(drm_cmdline_test_tv_option_secam_d),
	KUNIT_CASE(drm_cmdline_test_tv_option_secam_g),
	KUNIT_CASE(drm_cmdline_test_tv_option_secam_k),
	KUNIT_CASE(drm_cmdline_test_tv_option_secam_k1),
	KUNIT_CASE(drm_cmdline_test_tv_option_secam_l),
	KUNIT_CASE(drm_cmdline_test_tv_option_invalid),
	KUNIT_CASE(drm_cmdline_test_tv_option_truncated),
	KUNIT_CASE(drm_cmdline_test_rotate_0),
	KUNIT_CASE(drm_cmdline_test_rotate_90),
	KUNIT_CASE(drm_cmdline_test_rotate_180),
	KUNIT_CASE(drm_cmdline_test_rotate_270),
	KUNIT_CASE(drm_cmdline_test_rotate_multiple),
	KUNIT_CASE(drm_cmdline_test_rotate_invalid_val),
	KUNIT_CASE(drm_cmdline_test_rotate_truncated),
	KUNIT_CASE(drm_cmdline_test_hmirror),
	KUNIT_CASE(drm_cmdline_test_vmirror),
	KUNIT_CASE(drm_cmdline_test_margin_options),
	KUNIT_CASE(drm_cmdline_test_multiple_options),
	KUNIT_CASE(drm_cmdline_test_invalid_option),
	KUNIT_CASE(drm_cmdline_test_bpp_extra_and_option),
	KUNIT_CASE(drm_cmdline_test_extra_and_option),
	KUNIT_CASE(drm_cmdline_test_freestanding_options),
	KUNIT_CASE(drm_cmdline_test_freestanding_force_e_and_options),
	KUNIT_CASE(drm_cmdline_test_panel_orientation),
	{}
};

static struct kunit_suite drm_cmdline_parser_test_suite = {
	.name = "drm_cmdline_parser",
	.test_cases = drm_cmdline_parser_tests
};

kunit_test_suite(drm_cmdline_parser_test_suite);

MODULE_AUTHOR("Maxime Ripard <maxime.ripard@bootlin.com>");
MODULE_LICENSE("GPL");
