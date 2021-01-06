# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('wake-up-radio', ['core'])
    module.source = [
        'model/wur-net-device.cc',
        'model/wur-mac.cc',
        'model/wur-phy.cc',
        'model/wur-mode.cc',
        'model/wur-tx-vector.cc',
        'model/wur-net-device.cc',
        'model/wur-phy-state-helper.cc',
        #'helper/wake-up-radio-helper.cc',
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
        'model/wur-net-device.h',
        'model/wur-phy.h',
        'model/wur-mac.h',
        'model/wur-phy-band.h',
        'model/wur-mode.h',
        'model/wur-preamble.h',
        'model/wur-phy-state.h',
        'model/wur-mpdu-type.h',
        'model/wur-standards.h',
        'model/wur-tx-vector.h',
        'model/wur-error-rate-model.h',
        'model/wur-interference-helper.h',
        'model/wur-phy-state-helper.h',
        #'helper/wake-up-radio-helper.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

