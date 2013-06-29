#!/usr/bin/env python
# encoding: utf-8
"""
screengrab.py

Created by Alex Snet on 2011-10-10.
Copyright (c) 2011 CodeTeam. All rights reserved.
"""

import sys
import os

import Image


class screengrab:
    """if is wx :Need to create an App instance before doing anything"""
    def __init__(self):
        try:
            import gtk
        except ImportError:
            pass
        else:
            self.screen = self.getScreenByGtk

        try:
            import PyQt4
        except ImportError:
            pass
        else:
            self.screen = self.getScreenByQt

        try:
            import wx
        except ImportError:
            pass
        else:
            self.screen = self.getScreenByWx

        try:
            import ImageGrab
        except ImportError:
            pass
        else:
            self.screen = self.getScreenByPIL


    def getScreenByGtk(self):
        import gtk.gdk      
        w = gtk.gdk.get_default_root_window()
        sz = w.get_size()
        pb = gtk.gdk.Pixbuf(gtk.gdk.COLORSPACE_RGB,False,8,sz[0],sz[1])
        pb = pb.get_from_drawable(w,w.get_colormap(),0,0,0,0,sz[0],sz[1])
        if (pb != None):
            return False
        else:
            width,height = pb.get_width(),pb.get_height()
            return Image.fromstring("RGB",(width,height),pb.get_pixels() )

    def getScreenByQt(self):
        from PyQt4.QtGui import QPixmap, QApplication
        from PyQt4.Qt import QBuffer, QIODevice
        import StringIO
        app = QApplication(sys.argv)
        buffer = QBuffer()
        buffer.open(QIODevice.ReadWrite)
        QPixmap.grabWindow(QApplication.desktop().winId()).save(buffer, 'png')
        strio = StringIO.StringIO()
        strio.write(buffer.data())
        buffer.close()
        del app
        strio.seek(0)
        return Image.open(strio)

    def getScreenByPIL(self):
        import ImageGrab
        img = ImageGrab.grab()
        return img

    def getScreenByWx(self):
        import wx
        wx.App()  # Need to create an App instance before doing anything
        screen = wx.ScreenDC()
        size = screen.GetSize()
        bmp = wx.EmptyBitmap(size[0], size[1])
        mem = wx.MemoryDC(bmp)
        mem.Blit(0, 0, size[0], size[1], screen, 0, 0)
        del mem  # Release bitmap
        #bmp.SaveFile('screenshot.png', wx.BITMAP_TYPE_PNG)
        myWxImage = wx.ImageFromBitmap( myBitmap )
        PilImage = Image.new( 'RGB', (myWxImage.GetWidth(), myWxImage.GetHeight()) )
        PilImage.fromstring( myWxImage.GetData() )
        return PilImage

if __name__ == '__main__':
    s = screengrab()
    screen = s.screen()
    screen.show()
