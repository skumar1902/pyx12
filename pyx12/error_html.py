######################################################################
# Copyright (c) 2001-2004 Kalamazoo Community Mental Health Services,
#   John Holland <jholland@kazoocmh.org> <john@zoner.org>
# All rights reserved.
#
# This software is licensed as described in the file LICENSE.txt, which
# you should have received as part of this distribution.
#
######################################################################

#    $Id$

"""
Generates HTML error output
"""

#import os
#import sys
import string
from types import *
import time
import logging

# Intrapackage imports
from errors import *
#from utils import seg_str, escape_html_chars

__author__  = "John Holland <jholland@kazoocmh.org> <john@zoner.org>"

logger = logging.getLogger('pyx12.error_html')
logger.setLevel(logging.DEBUG)
#logger.setLevel(logging.ERROR)

class error_html:
    """
    """
    def __init__(self, errh, fd, term=('~', '*', '~', '\n')): 
        """
        @param fd: target file
        @type fd: file descriptor
        @param term: tuple of x12 terminators used
        @type term: tuple(string, string, string, string)

        @todo: GS errors are re-printing at the GE level
        @todo: Informational lines - Show loops
        """
        self.errh = errh
        self.fd = fd
        self.seg_term = term[0]
        self.ele_term = term[1]
        self.subele_term = term[2]
        self.eol = ''
        self.last_line = 0

    def header(self):
        self.fd.write('<html>\n<head>\n')
        self.fd.write('<title>X12N Error Analysis</title>\n')
        self.fd.write('<style type="text/css">\n<!--\n')
        self.fd.write('  span.seg { color: black; font-style: normal; }\n')
        self.fd.write('  span.error { background-color: #CCCCFF; color: red; font-style: normal; }\n')
        self.fd.write('  span.info { color: blue; font-style: normal; }\n')
        self.fd.write('  span.ele_err { background-color: yellow; color: red; font-style: normal; }\n')
        self.fd.write('  -->\n</style>\n')
        self.fd.write('  <link rel="stylesheet" href="errors.css" type="text/css">\n')
        self.fd.write('</head>\n<body>\n')
        self.fd.write('<h1>X12N Error Analysis</h1>\n<h3>Analysis Date: %s</h3><p>\n' % \
            (time.strftime('%m/%d/%Y %H:%M:%S')))
        self.fd.write('<div class="segs" style="">\n')

    def footer(self):
        self.fd.write('</div>\n')
        self.fd.write('<p>\n<a href="http://sourceforge.net/projects/pyx12/">pyx12 Validator</a>\n</p>\n')
        self.fd.write('</body>\n</html>\n')

    def gen_info(self, info_str):
        """
        """
        self.fd.write('<span class="info">&nbsp;&nbsp;%s</span><br>\n' % (info_str))
        
    def gen_seg(self, seg_data, src, err_node_list):
        """
        Params:     seg_data - data segment instance
        """
        cur_line = src.cur_line

        # Find error seg for this segment
        #   Find any skipped error values
        # ID pos of bad value
        #while errh
        ele_pos_map = {}
        for err_node in err_node_list:
            for ele in err_node.elements:
                ele_pos_map[ele.ele_pos] = ele.subele_pos

        t_seg = [] #list of formatted elements
        seg_data.format_ele_list(t_seg) 
        for i in range(len(seg_data)):
            if seg_data[i].is_composite():
                t_seg.append([])
                for j in range(len(seg_data[i])):
                    ele_str = escape_html_chars(seg_data[i][j].get_value())
                    if i in ele_pos_map.keys() and ele_pos_map[i] == j:
                        ele_str = self._wrap_ele_error(ele_str)
                    t_seg[-1].append(ele_str)
            else:
                ele_str = escape_html_chars(seg_data[i].get_value())
                if i in ele_pos_map.keys():
                    ele_str = self._wrap_ele_error(ele_str)
                t_seg.append(ele_str)
                
        for err_node in err_node_list:
            #for err_tuple in err_node.errors:
            for err_tuple in err_node.get_error_list(seg_data.get_seg_id(), True):
                err_cde = err_tuple[0]
                err_str = err_tuple[1]
                if err_cde == '3':
                    self.fd.write('<span class="error">&nbsp;%s (Segment Error Code: %s)</span><br>\n' % \
                        (err_str, err_cde))

        self.fd.write('<span class="seg">%i:&nbsp;%s</span><br>\n' % \
            (cur_line, self._seg_str(seg_data.get_seg_id(), t_seg)))
        for err_node in err_node_list:
            for err_tuple in err_node.get_error_list(seg_data.get_seg_id(), False):
            #for err_tuple in err_node.errors:
                err_cde = err_tuple[0]
                err_str = err_tuple[1]
                if err_cde != '3':
                    self.fd.write('<span class="error">&nbsp;%s (Segment Error Code: %s)</span><br>\n' % \
                        (err_str, err_cde))
            for ele in err_node.elements:
                for (err_cde, err_str, err_val) in ele.get_error_list(seg_data.get_seg_id(), False):
                #for (err_cde, err_str, err_val) in ele.errors:
                    self.fd.write('<span class="error">&nbsp;%s (Element Error Code: %s)</span><br>\n' % \
                        (err_str, err_cde))
        
    def _seg_str(self, seg_id, ele_list):
        """
        @param ele_list: list of formatted elements
        @rtype: string
        """
        return seg_id + self.ele_term + seg_str(ele_list, self.seg_term, self.ele_term, \
            self.subele_term, self.eol)
        
    def _wrap_ele_error(self, str1):
        """
        @rtype: string
        """
        return '<span class="ele_err">%s</span>' % (str1)


def seg_str(seg, seg_term, ele_term, subele_term, eol=''):
    """
    Join a list of elements
    @param seg: List of elements
    @type seg: list[string|list[string]]
    @param seg_term: Segment terminator character
    @type seg_term: string
    @param ele_term: Element terminator character
    @type ele_term: string
    @param subele_term: Sub-element terminator character
    @type subele_term: string
    @param eol: End of line character
    @type eol: string
    @return: formatted segment
    @rtype: string
    """
    #if None in seg:
    #    logger.debug(seg)
    tmp = []
    for a in seg:
        if type(a) is ListType:
            tmp.append(string.join(a, subele_term))
        else:
            tmp.append(a)
    return '%s%s%s' % (string.join(tmp, ele_term), seg_term, eol)

def escape_html_chars(str_val):
    """
    Escape special HTML characters (& <>)
    @type str_val: string
    @return: formatted string
    @rtype: string
    """
    if str_val is None: 
        return None
    output = str_val
    output = output.replace('&', '&amp;')
    output = output.replace(' ', '&nbsp;')
    output = output.replace('>', '&gt;')
    output = output.replace('<', '&lt;')
    return output
