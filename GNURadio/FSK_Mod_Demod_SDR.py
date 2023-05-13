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

from gnuradio import blocks
import pmt
from gnuradio import digital
from gnuradio import fec
from gnuradio import gr
from gnuradio.filter import firdes
from gnuradio.fft import window
import sys
import signal
from PyQt5 import Qt
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio import gr, pdu
from gnuradio import uhd
import time
from gnuradio.qtgui import Range, RangeWidget
from PyQt5 import QtCore
import FSK_Mod_Demod_SDR_epy_block_2 as epy_block_2  # embedded python block



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
        self.h = h = 1.2
        self.baud = baud = 80000
        self.vco_max = vco_max = 2500
        self.fsk_deviation = fsk_deviation = h*baud/2
        self.center = center = 2465230000
        self.vco_offset = vco_offset = (center-(fsk_deviation/2))/vco_max
        self.num_key = num_key = "packet_num"
        self.len_key = len_key = "packet_len"
        self.variable_repetition_encoder_def_0 = variable_repetition_encoder_def_0 = fec.repetition_encoder_make(2048, 3)
        self.variable_repetition_decoder_def_0 = variable_repetition_decoder_def_0 = fec.repetition_decoder.make(2048,3, 0.5)
        self.sq_lvl = sq_lvl = -15
        self.sps = sps = 2
        self.samp_rate = samp_rate = 220000
        self.repeat = repeat = 3520
        self.pack_len = pack_len = 20
        self.inp_amp = inp_amp = ((center+(fsk_deviation/2))/vco_max)-vco_offset
        self.hdr_format = hdr_format = digital.header_format_crc(len_key, num_key)
        self.delay = delay = 145

        ##################################################
        # Blocks
        ##################################################

        self.uhd_usrp_source_0_0_0 = uhd.usrp_source(
            ",".join(("serial=323098E", '')),
            uhd.stream_args(
                cpu_format="fc32",
                args='',
                channels=list(range(0,1)),
            ),
        )
        self.uhd_usrp_source_0_0_0.set_samp_rate(samp_rate)
        self.uhd_usrp_source_0_0_0.set_time_now(uhd.time_spec(time.time()), uhd.ALL_MBOARDS)

        self.uhd_usrp_source_0_0_0.set_center_freq(center, 0)
        self.uhd_usrp_source_0_0_0.set_antenna("TX/RX", 0)
        self.uhd_usrp_source_0_0_0.set_bandwidth((2*(fsk_deviation + baud)), 0)
        self.uhd_usrp_source_0_0_0.set_gain(40, 0)
        self.uhd_usrp_sink_0_0_0 = uhd.usrp_sink(
            ",".join(("serial=31E9D65", '')),
            uhd.stream_args(
                cpu_format="fc32",
                args='',
                channels=list(range(0,1)),
            ),
            "",
        )
        self.uhd_usrp_sink_0_0_0.set_samp_rate(samp_rate)
        self.uhd_usrp_sink_0_0_0.set_time_now(uhd.time_spec(time.time()), uhd.ALL_MBOARDS)

        self.uhd_usrp_sink_0_0_0.set_center_freq(center, 0)
        self.uhd_usrp_sink_0_0_0.set_antenna("TX/RX", 0)
        self.uhd_usrp_sink_0_0_0.set_bandwidth((2*(fsk_deviation + baud)), 0)
        self.uhd_usrp_sink_0_0_0.set_gain(120, 0)
        self._sq_lvl_range = Range(-100, 0, 5, -15, 200)
        self._sq_lvl_win = RangeWidget(self._sq_lvl_range, self.set_sq_lvl, "Squelch", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._sq_lvl_win)
        self.pdu_tagged_stream_to_pdu_0 = pdu.tagged_stream_to_pdu(gr.types.byte_t, 'packet_len')
        self.pdu_pdu_to_tagged_stream_0_1 = pdu.pdu_to_tagged_stream(gr.types.byte_t, 'packet_len')
        self.pdu_pdu_to_tagged_stream_0_0 = pdu.pdu_to_tagged_stream(gr.types.byte_t, 'packet_len')
        self.epy_block_2 = epy_block_2.blk()
        self.digital_protocol_parser_b_0 = digital.protocol_parser_b(hdr_format)
        self.digital_protocol_formatter_async_0 = digital.protocol_formatter_async(hdr_format)
        self.digital_header_payload_demux_0 = digital.header_payload_demux(
            32,
            1,
            0,
            "packet_len",
            "packet_len",
            False,
            gr.sizeof_char,
            "burst",
            int(samp_rate),
            (),
            0)
        self.digital_gfsk_mod_0 = digital.gfsk_mod(
            samples_per_symbol=2,
            sensitivity=1.0,
            bt=0.35,
            verbose=False,
            log=False,
            do_unpack=False)
        self.digital_gfsk_demod_0 = digital.gfsk_demod(
            samples_per_symbol=2,
            sensitivity=1.0,
            gain_mu=0.175,
            mu=0.5,
            omega_relative_limit=0.005,
            freq_error=0.0,
            verbose=False,
            log=False)
        self.digital_crc32_bb_0_0 = digital.crc32_bb(True, "packet_len", True)
        self.digital_crc32_bb_0 = digital.crc32_bb(False, "packet_len", True)
        self._delay_range = Range(0, 250, 1, 145, 200)
        self._delay_win = RangeWidget(self._delay_range, self.set_delay, "Delay", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._delay_win)
        self.blocks_tagged_stream_mux_0 = blocks.tagged_stream_mux(gr.sizeof_char*1, 'packet_len', 0)
        self.blocks_stream_to_tagged_stream_0_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, pack_len, "packet_len")
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, pack_len, "packet_len")
        self.blocks_repack_bits_bb_1_0 = blocks.repack_bits_bb(8, 1, "packet_len", False, gr.GR_MSB_FIRST)
        self.blocks_repack_bits_bb_1 = blocks.repack_bits_bb(1, 8, "packet_len", False, gr.GR_MSB_FIRST)
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_char*1, 'C:\\Users\\Drew\\Desktop\\torrent\\fskSDR\\originalMessage.txt', True, 0, 0)
        self.blocks_file_source_0.set_begin_tag(pmt.PMT_NIL)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_char*1, 'C:\\Users\\Drew\\Desktop\\torrent\\fskSDR\\demodMessage.txt', False)
        self.blocks_file_sink_0.set_unbuffered(False)


        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.digital_protocol_formatter_async_0, 'payload'), (self.pdu_pdu_to_tagged_stream_0_0, 'pdus'))
        self.msg_connect((self.digital_protocol_formatter_async_0, 'header'), (self.pdu_pdu_to_tagged_stream_0_1, 'pdus'))
        self.msg_connect((self.digital_protocol_parser_b_0, 'info'), (self.digital_header_payload_demux_0, 'header_data'))
        self.msg_connect((self.pdu_tagged_stream_to_pdu_0, 'pdus'), (self.digital_protocol_formatter_async_0, 'in'))
        self.connect((self.blocks_file_source_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))
        self.connect((self.blocks_repack_bits_bb_1, 0), (self.digital_crc32_bb_0_0, 0))
        self.connect((self.blocks_repack_bits_bb_1_0, 0), (self.digital_gfsk_mod_0, 0))
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.digital_crc32_bb_0, 0))
        self.connect((self.blocks_stream_to_tagged_stream_0_0, 0), (self.digital_header_payload_demux_0, 0))
        self.connect((self.blocks_tagged_stream_mux_0, 0), (self.blocks_repack_bits_bb_1_0, 0))
        self.connect((self.digital_crc32_bb_0, 0), (self.pdu_tagged_stream_to_pdu_0, 0))
        self.connect((self.digital_crc32_bb_0_0, 0), (self.blocks_file_sink_0, 0))
        self.connect((self.digital_crc32_bb_0_0, 0), (self.epy_block_2, 0))
        self.connect((self.digital_gfsk_demod_0, 0), (self.blocks_stream_to_tagged_stream_0_0, 0))
        self.connect((self.digital_gfsk_mod_0, 0), (self.uhd_usrp_sink_0_0_0, 0))
        self.connect((self.digital_header_payload_demux_0, 1), (self.blocks_repack_bits_bb_1, 0))
        self.connect((self.digital_header_payload_demux_0, 0), (self.digital_protocol_parser_b_0, 0))
        self.connect((self.pdu_pdu_to_tagged_stream_0_0, 0), (self.blocks_tagged_stream_mux_0, 1))
        self.connect((self.pdu_pdu_to_tagged_stream_0_1, 0), (self.blocks_tagged_stream_mux_0, 0))
        self.connect((self.uhd_usrp_source_0_0_0, 0), (self.digital_gfsk_demod_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "FSK_Mod_Demod_SDR")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_h(self):
        return self.h

    def set_h(self, h):
        self.h = h
        self.set_fsk_deviation(self.h*self.baud/2)

    def get_baud(self):
        return self.baud

    def set_baud(self, baud):
        self.baud = baud
        self.set_fsk_deviation(self.h*self.baud/2)
        self.uhd_usrp_sink_0_0_0.set_bandwidth((2*(self.fsk_deviation + self.baud)), 0)
        self.uhd_usrp_source_0_0_0.set_bandwidth((2*(self.fsk_deviation + self.baud)), 0)

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
        self.uhd_usrp_sink_0_0_0.set_bandwidth((2*(self.fsk_deviation + self.baud)), 0)
        self.uhd_usrp_source_0_0_0.set_bandwidth((2*(self.fsk_deviation + self.baud)), 0)

    def get_center(self):
        return self.center

    def set_center(self, center):
        self.center = center
        self.set_inp_amp(((self.center+(self.fsk_deviation/2))/self.vco_max)-self.vco_offset)
        self.set_vco_offset((self.center-(self.fsk_deviation/2))/self.vco_max)
        self.uhd_usrp_sink_0_0_0.set_center_freq(self.center, 0)
        self.uhd_usrp_source_0_0_0.set_center_freq(self.center, 0)

    def get_vco_offset(self):
        return self.vco_offset

    def set_vco_offset(self, vco_offset):
        self.vco_offset = vco_offset
        self.set_inp_amp(((self.center+(self.fsk_deviation/2))/self.vco_max)-self.vco_offset)

    def get_num_key(self):
        return self.num_key

    def set_num_key(self, num_key):
        self.num_key = num_key
        self.set_hdr_format(digital.header_format_crc(self.len_key, self.num_key))

    def get_len_key(self):
        return self.len_key

    def set_len_key(self, len_key):
        self.len_key = len_key
        self.set_hdr_format(digital.header_format_crc(self.len_key, self.num_key))

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

    def get_sps(self):
        return self.sps

    def set_sps(self, sps):
        self.sps = sps

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_1.set_sample_rate((self.samp_rate-50000))
        self.uhd_usrp_sink_0_0_0.set_samp_rate(self.samp_rate)
        self.uhd_usrp_source_0_0_0.set_samp_rate(self.samp_rate)

    def get_repeat(self):
        return self.repeat

    def set_repeat(self, repeat):
        self.repeat = repeat

    def get_pack_len(self):
        return self.pack_len

    def set_pack_len(self, pack_len):
        self.pack_len = pack_len
        self.blocks_stream_to_tagged_stream_0.set_packet_len(self.pack_len)
        self.blocks_stream_to_tagged_stream_0.set_packet_len_pmt(self.pack_len)
        self.blocks_stream_to_tagged_stream_0_0.set_packet_len(self.pack_len)
        self.blocks_stream_to_tagged_stream_0_0.set_packet_len_pmt(self.pack_len)

    def get_inp_amp(self):
        return self.inp_amp

    def set_inp_amp(self, inp_amp):
        self.inp_amp = inp_amp

    def get_hdr_format(self):
        return self.hdr_format

    def set_hdr_format(self, hdr_format):
        self.hdr_format = hdr_format

    def get_delay(self):
        return self.delay

    def set_delay(self, delay):
        self.delay = delay




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
