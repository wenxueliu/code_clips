#! /usr/bin/env python
# -*- coding:utf-8 -*-

#fileName  : MergeImage.py
#Author    : Wenxueliu 
#Date      : 2013-05-18

import os
import sys
import Image
import time

def make_thumb(rate, path, thumb_path):
    """thumb image 
       if rate < 1 will shrink 
       else extend  
    """
    img = Image.open(path)
    width, height = img.size
    width = width.rate
    height = height.rate

    region = img

    thumb = region.resize((width, height), Image.ANTIALIAS)
    base, ext = os.path.splitext(os.path.basename(path))
    filename = os.path.join(thumb_path, "%s_thumb.jpg"%(base,))
    thumb.save(filename, quality=70)


class MergeImg:
     """
         if mode = 0 will range in height
         if mode = 1 will range in width 
         if mode = 2 will range sys.argv[1] of cols
     """
     
     BORDER_SIZE = 0
     COLS = 5
     ROWS = 1
     WIDTHEST = 0
     HEIGHTEST = 0
     def __init__(self, imageFiles, mode):
         self.images = []
         self.num = len(imageFiles)
         self.mode = mode
         self.width = 0
         self.height = 0
         if self.mode == 0:
             #get the most width image as width 
             for image in imageFiles:
                 im = Image.open(image)
                 if im.size[0] > self.width:
                     self.width = im.size[0]
                 self.height = self.height + im.size[1]
                 self.images.append(im)
             #the width and height of merge image
             self.width = self.width + 2 * self.BORDER_SIZE
             self.height = self.height + (self.num + 1) * self.BORDER_SIZE
         elif self.mode == 1:
             #get the most height image as height 
             for image in imageFiles:
                 im = Image.open(image)
                 if im.size[1] > self.height:
                     self.height = im.size[1]
                 self.width = self.width + im.size[0]
                 self.images.append(im)
             #the width and height of merge image
             self.width = self.width + (self.num + 1) * self.BORDER_SIZE
             self.height = self.height +  2 * self.BORDER_SIZE
         elif self.mode == 2:
             #get the most height image as height 
             for image in imageFiles:
                 im = Image.open(image)
                 if im.size[0] > self.WIDTHEST:
                     self.WIDTHEST = im.size[0]
                 if im.size[1] > self.HEIGHTEST:
                     self.HEIGHTEST = im.size[1]
                 #self.width = self.width + im.size[0]
                 self.images.append(im)
             #the width and height of merge image
             self.width = self.WIDTHEST * self.COLS + (self.COLS+1) * self.BORDER_SIZE 
             self.ROWS = int((self.num-1) / self.COLS) + 1
             self.height = self.HEIGHTEST * self.ROWS + (self.ROWS+1) * self.BORDER_SIZE

     def save(self, fileName):
         self.outImg = Image.new('RGB', (self.width, self.height))
         self.outImg.paste(0xFFFFFF, (0, 0, self.width, self.height))
         offset = 0
         if self.mode == 0:
             curHeight = self.BORDER_SIZE
             for image in self.images:
                 tmp = image.copy()
                 offset = (self.width - 2 * self.BORDER_SIZE - image.size[0]) / 2
                 self.outImg.paste(tmp, (self.BORDER_SIZE + offset, curHeight))
                 curHeight = curHeight + tmp.size[1] + self.BORDER_SIZE
         elif self.mode == 1:
             curWidth = self.BORDER_SIZE
             for image in self.images:
                 tmp = image.copy()
                 offset = (self.height - 2 * self.BORDER_SIZE - image.size[1]) / 2
                 self.outImg.paste(tmp, (curWidth, self.BORDER_SIZE + offset))
                 curWidth = curWidth + tmp.size[0] + self.BORDER_SIZE
         elif self.mode == 2:
             curWidth = self.BORDER_SIZE 
             curHeight = self.BORDER_SIZE 
             imgs = [img.copy() for img in self.images] 
             imgLen = len(imgs)
             for i in xrange(imgLen):
                 offsetWidth = (self.WIDTHEST - imgs[i].size[0]) / 2
                 offsetHeight = (self.HEIGHTEST - imgs[i].size[1]) / 2
                 self.outImg.paste(imgs[i], (curWidth+offsetWidth, curHeight +
                             offsetHeight))
                 curWidth = self.BORDER_SIZE + (self.WIDTHEST + self.BORDER_SIZE) * ((i+1)%self.COLS)
                 curHeight = self.BORDER_SIZE + (self.HEIGHTEST + self.BORDER_SIZE) * ((i+1)/self.COLS)
         self.outImg.save(fileName)
            
def main():
    if len(sys.argv) < 2:
        print "uasge: %s [imgesfile]" %sys.argv[0]
        sys.exit(1)
    TIMEFORMAT = '%Y%m%d%H%M%S'
    outFile = 'Py' + time.strftime(TIMEFORMAT,
            time.localtime()) + '.jpg'
    imageFiles = sys.argv[1:]
    mergeImg = MergeImg(imageFiles, 2)
    mergeImg.save(outFile)
    os.system(outFile)
if __name__ == '__main__':
    main()
      

