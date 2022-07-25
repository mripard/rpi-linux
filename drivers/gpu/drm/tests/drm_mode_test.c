// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022 Maxime Ripard <mripard@kernel.org>
 */

#include <drm/drm_mode.h>
#include <kunit/test.h>

#include <drm/drm_connector.h>
#include <drm/drm_edid.h>
#include <drm/drm_drv.h>
#include <drm/drm_modes.h>
#include <drm/drm_modeset_helper_vtables.h>
#include <drm/drm_probe_helper.h>

struct drm_mode_test_priv {
	struct device *dev;
	struct drm_device *drm;
	struct drm_connector connector;
};

static const struct drm_mode_config_funcs drm_mode_config_funcs = {
};

static const struct drm_driver drm_mode_driver = {
};

static int drm_mode_connector_get_modes(struct drm_connector *connector)
{
	struct drm_display_mode *mode;
	int ret;

	ret = drm_add_modes_noedid(connector, 1920, 1200);
	if (ret)
		return ret;

	mode = drm_mode_duplicate(connector->dev, &drm_mode_480i);
	if (!mode)
		return -ENOMEM;

	drm_mode_probed_add(connector, mode);

	mode = drm_mode_duplicate(connector->dev, &drm_mode_576i);
	if (!mode)
		return -ENOMEM;

	drm_mode_probed_add(connector, mode);

	return 0;
}

static const struct drm_connector_helper_funcs drm_mode_connector_helper_funcs = {
	.get_modes = drm_mode_connector_get_modes,
};

static const struct drm_connector_funcs drm_mode_connector_funcs = {
};

static int drm_mode_test_init(struct kunit *test)
{
	struct drm_mode_test_priv *priv;
	int ret;

	priv = kunit_kzalloc(test, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;
	test->priv = priv;

	priv->dev = root_device_register("drm-mode-test");
	if (IS_ERR(priv->dev))
		return PTR_ERR(priv->dev);

	priv->drm = drm_dev_alloc(&drm_mode_driver, priv->dev);
	if (IS_ERR(priv->drm))
		return PTR_ERR(priv->drm);
	priv->drm->mode_config.funcs = &drm_mode_config_funcs;

	ret = drmm_mode_config_init(priv->drm);
	if (ret)
		return ret;

	ret = drmm_connector_init(priv->drm, &priv->connector,
				  &drm_mode_connector_funcs,
				  DRM_MODE_CONNECTOR_Unknown,
				  NULL);
	if (ret)
		return ret;
	drm_connector_helper_add(&priv->connector, &drm_mode_connector_helper_funcs);

	priv->connector.interlace_allowed = true;
	priv->connector.doublescan_allowed = true;

	return 0;
}

static void drm_mode_test_exit(struct kunit *test)
{
	struct drm_mode_test_priv *priv = test->priv;

	drm_dev_put(priv->drm);
	root_device_unregister(priv->dev);
}

static void drm_mode_res_1920_1080_60(struct kunit *test)
{
	struct drm_mode_test_priv *priv = test->priv;
	struct drm_device *drm = priv->drm;
	struct drm_connector *connector = &priv->connector;
	struct drm_cmdline_mode *cmdline_mode = &connector->cmdline_mode;
	struct drm_display_mode *expected_mode, *mode;
	const char *cmdline = "1920x1080@60";
	int ret;

	expected_mode = drm_mode_find_dmt(priv->drm, 1920, 1080, 60, false);
	KUNIT_ASSERT_PTR_NE(test, expected_mode, NULL);

	KUNIT_ASSERT_TRUE(test,
			  drm_mode_parse_command_line_for_connector(cmdline,
								    connector,
								    cmdline_mode));

	mutex_lock(&drm->mode_config.mutex);
	ret = drm_helper_probe_single_connector_modes(connector, 1920, 1080);
	mutex_unlock(&drm->mode_config.mutex);
	KUNIT_ASSERT_GT(test, ret, 0);

	mode = drm_connector_pick_cmdline_mode(connector);
	KUNIT_ASSERT_PTR_NE(test, mode, NULL);

	KUNIT_EXPECT_TRUE(test, drm_mode_equal(expected_mode, mode));
}

static void drm_mode_named_ntsc(struct kunit *test)
{
	struct drm_mode_test_priv *priv = test->priv;
	struct drm_device *drm = priv->drm;
	struct drm_connector *connector = &priv->connector;
	struct drm_cmdline_mode *cmdline_mode = &connector->cmdline_mode;
	struct drm_display_mode *mode;
	const char *cmdline = "NTSC";
	int ret;

	KUNIT_ASSERT_TRUE(test,
			  drm_mode_parse_command_line_for_connector(cmdline,
								    connector,
								    cmdline_mode));

	mutex_lock(&drm->mode_config.mutex);
	ret = drm_helper_probe_single_connector_modes(connector, 1920, 1080);
	mutex_unlock(&drm->mode_config.mutex);
	KUNIT_ASSERT_GT(test, ret, 0);

	mode = drm_connector_pick_cmdline_mode(connector);
	KUNIT_ASSERT_PTR_NE(test, mode, NULL);

	KUNIT_EXPECT_TRUE(test, drm_mode_equal(&drm_mode_480i, mode));
}

static void drm_mode_named_ntsc_j(struct kunit *test)
{
	struct drm_mode_test_priv *priv = test->priv;
	struct drm_device *drm = priv->drm;
	struct drm_connector *connector = &priv->connector;
	struct drm_cmdline_mode *cmdline_mode = &connector->cmdline_mode;
	struct drm_display_mode *mode;
	const char *cmdline = "NTSC_J";
	int ret;

	KUNIT_ASSERT_TRUE(test,
			  drm_mode_parse_command_line_for_connector(cmdline,
								    connector,
								    cmdline_mode));

	mutex_lock(&drm->mode_config.mutex);
	ret = drm_helper_probe_single_connector_modes(connector, 1920, 1080);
	mutex_unlock(&drm->mode_config.mutex);
	KUNIT_ASSERT_GT(test, ret, 0);

	mode = drm_connector_pick_cmdline_mode(connector);
	KUNIT_ASSERT_PTR_NE(test, mode, NULL);

	KUNIT_EXPECT_TRUE(test, drm_mode_equal(&drm_mode_480i, mode));
}

static void drm_mode_named_pal(struct kunit *test)
{
	struct drm_mode_test_priv *priv = test->priv;
	struct drm_device *drm = priv->drm;
	struct drm_connector *connector = &priv->connector;
	struct drm_cmdline_mode *cmdline_mode = &connector->cmdline_mode;
	struct drm_display_mode *mode;
	const char *cmdline = "PAL";
	int ret;

	KUNIT_ASSERT_TRUE(test,
			  drm_mode_parse_command_line_for_connector(cmdline,
								    connector,
								    cmdline_mode));

	mutex_lock(&drm->mode_config.mutex);
	ret = drm_helper_probe_single_connector_modes(connector, 1920, 1080);
	mutex_unlock(&drm->mode_config.mutex);
	KUNIT_ASSERT_GT(test, ret, 0);

	mode = drm_connector_pick_cmdline_mode(connector);
	KUNIT_ASSERT_PTR_NE(test, mode, NULL);

	KUNIT_EXPECT_TRUE(test, drm_mode_equal(&drm_mode_576i, mode));
}

static void drm_mode_named_pal_m(struct kunit *test)
{
	struct drm_mode_test_priv *priv = test->priv;
	struct drm_device *drm = priv->drm;
	struct drm_connector *connector = &priv->connector;
	struct drm_cmdline_mode *cmdline_mode = &connector->cmdline_mode;
	struct drm_display_mode *mode;
	const char *cmdline = "PAL_M";
	int ret;

	KUNIT_ASSERT_TRUE(test,
			  drm_mode_parse_command_line_for_connector(cmdline,
								    connector,
								    cmdline_mode));

	mutex_lock(&drm->mode_config.mutex);
	ret = drm_helper_probe_single_connector_modes(connector, 1920, 1080);
	mutex_unlock(&drm->mode_config.mutex);
	KUNIT_ASSERT_GT(test, ret, 0);

	mode = drm_connector_pick_cmdline_mode(connector);
	KUNIT_ASSERT_PTR_NE(test, mode, NULL);

	KUNIT_EXPECT_TRUE(test, drm_mode_equal(&drm_mode_480i, mode));
}

static struct kunit_case drm_mode_tests[] = {
	KUNIT_CASE(drm_mode_res_1920_1080_60),
	KUNIT_CASE(drm_mode_named_ntsc),
	KUNIT_CASE(drm_mode_named_ntsc_j),
	KUNIT_CASE(drm_mode_named_pal),
	KUNIT_CASE(drm_mode_named_pal_m),
	{}
};

static struct kunit_suite drm_mode_test_suite = {
	.name = "drm_mode",
	.init = drm_mode_test_init,
	.exit = drm_mode_test_exit,
	.test_cases = drm_mode_tests
};

kunit_test_suite(drm_mode_test_suite);
MODULE_AUTHOR("Maxime Ripard <mripard@kernel.org>");
MODULE_LICENSE("GPL");
