# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('wake-up-radio', ['core'])
    module.source = [
        'model/wur-main-radio-net-device.cc',
        'model/wur-main-radio-net-device-channel.cc',
        'model/wur-main-radio-net-device-phy.cc',
        'model/wur-main-radio-net-device-phy-state-helper.cc',
        'model/wur-main-radio-ppdu.cc',
        ]

    module_test = bld.create_ns3_module_test_library('wake-up-radio')
    module_test.source = [
        #'test/wake-up-radio-test-suite.cc',
        ]
    # Tests encapsulating example programs should be listed here
    if (bld.env['ENABLE_EXAMPLES']):
        module_test.source.extend([
        #    'test/wake-up-radio-examples-test-suite.cc',
             ])

    headers = bld(features='ns3header')
    headers.module = 'wake-up-radio'
    headers.source = [
        'model/wur-main-radio-net-device.h',
        'model/wur-main-radio-net-device-phy.h',
        'model/wur-main-radio-net-device-phy-state-helper.h',
        'model/wur-main-radio-net-device-channel.h',
        'model/wur-main-radio-ppdu.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

