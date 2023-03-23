#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: FSK_Mod_Demod_SDR
# Author: Drew Pearlstein
# Description: FSK_Mod_Demod updated
# GNU Radio version: 3.10.5.1

from packaging.version import Version as StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print("Warning: failed to XInitThreads()")

from PyQt5 import Qt
from gnuradio import qtgui
from gnuradio.filter import firdes
import sip
from gnuradio import analog
import math
from gnuradio import blocks
import pmt
from gnuradio import digital
from gnuradio import fec
from gnuradio import filter
from gnuradio import gr
from gnuradio.fft import window
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio import uhd
import time
from gnuradio.qtgui import Range, RangeWidget
from PyQt5 import QtCore



from gnuradio import qtgui

class FSK_Mod_Demod_SDR(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "FSK_Mod_Demod_SDR", catch_exceptions=True)
        Qt.QWidget.__init__(self)
        self.setWindowTitle("FSK_Mod_Demod_SDR")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "FSK_Mod_Demod_SDR")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except:
            pass

        ##################################################
        # Variables
        ##################################################
        self.vco_max = vco_max = 2500
        self.fsk_deviation = fsk_deviation = 170
        self.center = center = 2400000000
        self.vco_offset = vco_offset = (center-(fsk_deviation/2))/vco_max
        self.samp_rate = samp_rate = 400000
        self.variable_repetition_encoder_def_0 = variable_repetition_encoder_def_0 = fec.repetition_encoder_make(2048, 3)
        self.variable_repetition_decoder_def_0 = variable_repetition_decoder_def_0 = fec.repetition_decoder.make(2048,3, 0.5)
        self.sq_lvl = sq_lvl = -70
        self.sps = sps = 11
        self.repeat = repeat = (int)(samp_rate*0.022)
        self.inp_amp = inp_amp = ((center+(fsk_deviation/2))/vco_max)-vco_offset
        self.delay = delay = 145
        self.baud = baud = 1/0.022

        ##################################################
        # Blocks
        ##################################################

        self._sq_lvl_range = Range(-100, 0, 5, -70, 200)
        self._sq_lvl_win = RangeWidget(self._sq_lvl_range, self.set_sq_lvl, "Squelch", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._sq_lvl_win)
        self._delay_range = Range(0, 250, 1, 145, 200)
        self._delay_win = RangeWidget(self._delay_range, self.set_delay, "Delay", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._delay_win)
        self.uhd_usrp_source_0 = uhd.usrp_source(
            ",".join(("serial=323098E", '')),
            uhd.stream_args(
                cpu_format="fc32",
                args='',
                channels=list(range(0,1)),
            ),
        )
        self.uhd_usrp_source_0.set_samp_rate(samp_rate)
        self.uhd_usrp_source_0.set_time_now(uhd.time_spec(time.time()), uhd.ALL_MBOARDS)

        self.uhd_usrp_source_0.set_center_freq(center, 0)
        self.uhd_usrp_source_0.set_antenna("TX/RX", 0)
        self.uhd_usrp_source_0.set_gain(30, 0)
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
            ",".join(("serial=31E9D65", '')),
            uhd.stream_args(
                cpu_format="fc32",
                args='',
                channels=list(range(0,1)),
            ),
            "",
        )
        self.uhd_usrp_sink_0.set_samp_rate(samp_rate)
        self.uhd_usrp_sink_0.set_time_now(uhd.time_spec(time.time()), uhd.ALL_MBOARDS)

        self.uhd_usrp_sink_0.set_center_freq(center, 0)
        self.uhd_usrp_sink_0.set_antenna("TX/RX", 0)
        self.uhd_usrp_sink_0.set_gain(30, 0)
        self.rational_resampler_xxx_0_0 = filter.rational_resampler_fff(
                interpolation=1,
                decimation=96,
                taps=[],
                fractional_bw=0)
        self.qtgui_time_sink_x_0 = qtgui.time_sink_f(
            16384, #size
            samp_rate, #samp_rate
            "", #name
            3, #number of inputs
            None # parent
        )
        self.qtgui_time_sink_x_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0.enable_tags(True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_NORM, qtgui.TRIG_SLOPE_NEG, 0.5, 0, 0, "")
        self.qtgui_time_sink_x_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0.enable_grid(False)
        self.qtgui_time_sink_x_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0.enable_control_panel(True)
        self.qtgui_time_sink_x_0.enable_stem_plot(False)


        labels = ['Signal 1', 'Signal 2', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(3):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.qwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_win)
        self.qtgui_eye_sink_x_0 = qtgui.eye_sink_f(
            1024, #size
            500, #samp_rate
            1, #number of inputs
            None
        )
        self.qtgui_eye_sink_x_0.set_update_time(0.10)
        self.qtgui_eye_sink_x_0.set_samp_per_symbol(sps)
        self.qtgui_eye_sink_x_0.set_y_axis(-1, 1)

        self.qtgui_eye_sink_x_0.set_y_label('Amplitude', "")

        self.qtgui_eye_sink_x_0.enable_tags(True)
        self.qtgui_eye_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_AUTO, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, '')
        self.qtgui_eye_sink_x_0.enable_autoscale(False)
        self.qtgui_eye_sink_x_0.enable_grid(False)
        self.qtgui_eye_sink_x_0.enable_axis_labels(True)
        self.qtgui_eye_sink_x_0.enable_control_panel(False)


        labels = ['Signal 1', 'Signal 2', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'blue', 'blue', 'blue', 'blue',
            'blue', 'blue', 'blue', 'blue', 'blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(1):
            if len(labels[i]) == 0:
                self.qtgui_eye_sink_x_0.set_line_label(i, "Eye[Data {0}]".format(i))
            else:
                self.qtgui_eye_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_eye_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_eye_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_eye_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_eye_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_eye_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_eye_sink_x_0_win = sip.wrapinstance(self.qtgui_eye_sink_x_0.qwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_eye_sink_x_0_win)
        self.freq_xlating_fir_filter_xxx_0 = filter.freq_xlating_fir_filter_ccf(1, firdes.low_pass(1.0,samp_rate,1000,400), center, samp_rate)
        self.fec_generic_encoder_0 = fec.encoder(variable_repetition_encoder_def_0, gr.sizeof_char, gr.sizeof_float)
        self.fec_generic_decoder_0 = fec.decoder(variable_repetition_decoder_def_0, gr.sizeof_gr_complex, gr.sizeof_char)
        self.digital_binary_slicer_fb_0 = digital.binary_slicer_fb()
        self.blocks_vco_c_0 = blocks.vco_c(samp_rate, 15708, .5)
        self.blocks_unpacked_to_packed_xx_0_2 = blocks.unpacked_to_packed_bb(8, gr.GR_MSB_FIRST)
        self.blocks_unpacked_to_packed_xx_0_0 = blocks.unpacked_to_packed_bb(8, gr.GR_MSB_FIRST)
        self.blocks_unpack_k_bits_bb_0 = blocks.unpack_k_bits_bb(8)
        self.blocks_uchar_to_float_0_0 = blocks.uchar_to_float()
        self.blocks_sub_xx_0 = blocks.sub_ff(1)
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_ff(inp_amp)
        self.blocks_float_to_uchar_1 = blocks.float_to_uchar()
        self.blocks_float_to_uchar_0 = blocks.float_to_uchar()
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_char*1, 'C:\\Users\\Drew\\Desktop\\torrent\\fskSDR\\originalMessage.txt', False, 0, 0)
        self.blocks_file_source_0.set_begin_tag(pmt.PMT_NIL)
        self.blocks_file_sink_1_0 = blocks.file_sink(gr.sizeof_char*1, 'C:\\Users\\Drew\\Desktop\\torrent\\fskSDR\\testMessage1.txt', False)
        self.blocks_file_sink_1_0.set_unbuffered(False)
        self.blocks_file_sink_1 = blocks.file_sink(gr.sizeof_char*1, 'C:\\Users\\Drew\\Desktop\\torrent\\fskSDR\\replica.txt', False)
        self.blocks_file_sink_1.set_unbuffered(False)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_char*1, 'C:\\Users\\Drew\\Desktop\\torrent\\fskSDR\\demodMessage.txt', False)
        self.blocks_file_sink_0.set_unbuffered(False)
        self.blocks_delay_0 = blocks.delay(gr.sizeof_float*1, int(delay))
        self.blocks_add_const_vxx_1_2 = blocks.add_const_bb(48)
        self.blocks_add_const_vxx_1_0 = blocks.add_const_bb(48)
        self.blocks_add_const_vxx_0 = blocks.add_const_ff(vco_offset)
        self.analog_simple_squelch_cc_0 = analog.simple_squelch_cc(sq_lvl, 1)
        self.analog_quadrature_demod_cf_1 = analog.quadrature_demod_cf((samp_rate/(2*math.pi*fsk_deviation)))


        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_quadrature_demod_cf_1, 0), (self.digital_binary_slicer_fb_0, 0))
        self.connect((self.analog_quadrature_demod_cf_1, 0), (self.rational_resampler_xxx_0_0, 0))
        self.connect((self.analog_simple_squelch_cc_0, 0), (self.analog_quadrature_demod_cf_1, 0))
        self.connect((self.blocks_add_const_vxx_0, 0), (self.blocks_vco_c_0, 0))
        self.connect((self.blocks_add_const_vxx_1_0, 0), (self.blocks_file_sink_0, 0))
        self.connect((self.blocks_add_const_vxx_1_2, 0), (self.blocks_file_sink_1, 0))
        self.connect((self.blocks_delay_0, 0), (self.blocks_sub_xx_0, 1))
        self.connect((self.blocks_delay_0, 0), (self.qtgui_time_sink_x_0, 2))
        self.connect((self.blocks_file_source_0, 0), (self.blocks_unpack_k_bits_bb_0, 0))
        self.connect((self.blocks_float_to_uchar_0, 0), (self.blocks_unpacked_to_packed_xx_0_2, 0))
        self.connect((self.blocks_float_to_uchar_1, 0), (self.blocks_unpacked_to_packed_xx_0_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.blocks_add_const_vxx_0, 0))
        self.connect((self.blocks_sub_xx_0, 0), (self.qtgui_time_sink_x_0, 1))
        self.connect((self.blocks_uchar_to_float_0_0, 0), (self.blocks_float_to_uchar_1, 0))
        self.connect((self.blocks_uchar_to_float_0_0, 0), (self.blocks_sub_xx_0, 0))
        self.connect((self.blocks_uchar_to_float_0_0, 0), (self.qtgui_time_sink_x_0, 0))
        self.connect((self.blocks_unpack_k_bits_bb_0, 0), (self.fec_generic_encoder_0, 0))
        self.connect((self.blocks_unpacked_to_packed_xx_0_0, 0), (self.blocks_add_const_vxx_1_0, 0))
        self.connect((self.blocks_unpacked_to_packed_xx_0_2, 0), (self.blocks_add_const_vxx_1_2, 0))
        self.connect((self.blocks_vco_c_0, 0), (self.fec_generic_decoder_0, 0))
        self.connect((self.blocks_vco_c_0, 0), (self.uhd_usrp_sink_0, 0))
        self.connect((self.digital_binary_slicer_fb_0, 0), (self.blocks_uchar_to_float_0_0, 0))
        self.connect((self.fec_generic_decoder_0, 0), (self.blocks_file_sink_1_0, 0))
        self.connect((self.fec_generic_encoder_0, 0), (self.blocks_delay_0, 0))
        self.connect((self.fec_generic_encoder_0, 0), (self.blocks_float_to_uchar_0, 0))
        self.connect((self.fec_generic_encoder_0, 0), (self.blocks_multiply_const_vxx_0, 0))
        self.connect((self.freq_xlating_fir_filter_xxx_0, 0), (self.analog_simple_squelch_cc_0, 0))
        self.connect((self.rational_resampler_xxx_0_0, 0), (self.qtgui_eye_sink_x_0, 0))
        self.connect((self.uhd_usrp_source_0, 0), (self.freq_xlating_fir_filter_xxx_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "FSK_Mod_Demod_SDR")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_vco_max(self):
        return self.vco_max

    def set_vco_max(self, vco_max):
        self.vco_max = vco_max
        self.set_inp_amp(((self.center+(self.fsk_deviation/2))/self.vco_max)-self.vco_offset)
        self.set_vco_offset((self.center-(self.fsk_deviation/2))/self.vco_max)

    def get_fsk_deviation(self):
        return self.fsk_deviation

    def set_fsk_deviation(self, fsk_deviation):
        self.fsk_deviation = fsk_deviation
        self.set_inp_amp(((self.center+(self.fsk_deviation/2))/self.vco_max)-self.vco_offset)
        self.set_vco_offset((self.center-(self.fsk_deviation/2))/self.vco_max)
        self.analog_quadrature_demod_cf_1.set_gain((self.samp_rate/(2*math.pi*self.fsk_deviation)))

    def get_center(self):
        return self.center

    def set_center(self, center):
        self.center = center
        self.set_inp_amp(((self.center+(self.fsk_deviation/2))/self.vco_max)-self.vco_offset)
        self.set_vco_offset((self.center-(self.fsk_deviation/2))/self.vco_max)
        self.freq_xlating_fir_filter_xxx_0.set_center_freq(self.center)
        self.uhd_usrp_sink_0.set_center_freq(self.center, 0)
        self.uhd_usrp_source_0.set_center_freq(self.center, 0)

    def get_vco_offset(self):
        return self.vco_offset

    def set_vco_offset(self, vco_offset):
        self.vco_offset = vco_offset
        self.set_inp_amp(((self.center+(self.fsk_deviation/2))/self.vco_max)-self.vco_offset)
        self.blocks_add_const_vxx_0.set_k(self.vco_offset)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_repeat((int)(self.samp_rate*0.022))
        self.analog_quadrature_demod_cf_1.set_gain((self.samp_rate/(2*math.pi*self.fsk_deviation)))
        self.freq_xlating_fir_filter_xxx_0.set_taps(firdes.low_pass(1.0,self.samp_rate,1000,400))
        self.qtgui_time_sink_x_0.set_samp_rate(self.samp_rate)
        self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate)
        self.uhd_usrp_source_0.set_samp_rate(self.samp_rate)

    def get_variable_repetition_encoder_def_0(self):
        return self.variable_repetition_encoder_def_0

    def set_variable_repetition_encoder_def_0(self, variable_repetition_encoder_def_0):
        self.variable_repetition_encoder_def_0 = variable_repetition_encoder_def_0

    def get_variable_repetition_decoder_def_0(self):
        return self.variable_repetition_decoder_def_0

    def set_variable_repetition_decoder_def_0(self, variable_repetition_decoder_def_0):
        self.variable_repetition_decoder_def_0 = variable_repetition_decoder_def_0

    def get_sq_lvl(self):
        return self.sq_lvl

    def set_sq_lvl(self, sq_lvl):
        self.sq_lvl = sq_lvl
        self.analog_simple_squelch_cc_0.set_threshold(self.sq_lvl)

    def get_sps(self):
        return self.sps

    def set_sps(self, sps):
        self.sps = sps
        self.qtgui_eye_sink_x_0.set_samp_per_symbol(self.sps)

    def get_repeat(self):
        return self.repeat

    def set_repeat(self, repeat):
        self.repeat = repeat

    def get_inp_amp(self):
        return self.inp_amp

    def set_inp_amp(self, inp_amp):
        self.inp_amp = inp_amp
        self.blocks_multiply_const_vxx_0.set_k(self.inp_amp)

    def get_delay(self):
        return self.delay

    def set_delay(self, delay):
        self.delay = delay
        self.blocks_delay_0.set_dly(int(int(self.delay)))

    def get_baud(self):
        return self.baud

    def set_baud(self, baud):
        self.baud = baud




def main(top_block_cls=FSK_Mod_Demod_SDR, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()

    tb.start()

    tb.show()

    def sig_handler(sig=None, frame=None):
        tb.stop()
        tb.wait()

        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    qapp.exec_()

if __name__ == '__main__':
    main()
