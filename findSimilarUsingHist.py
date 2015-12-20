# -*- coding:UTF8 -*-
import os
import sys
import cv2
import numpy as np

if __name__ == '__main__':
    reload(sys)
    sys.setdefaultencoding( "utf-8" )
    InPath = r'D:\lzhi\work\picture\101_ObjectCategories\water_lilly'
    uInPath = unicode(InPath, 'utf8')
    ShowW = 256
    ShowH = 256
    cv2.namedWindow('Img', cv2.WINDOW_AUTOSIZE)
    for filename in os.listdir(uInPath):
        Fullfile = os.path.join(uInPath, filename)
        print Fullfile
        Fullfile = Fullfile.decode('utf8').encode('gbk')
        cvImgOri = cv2.imread(Fullfile)
        cvImgShow = cv2.resize(cvImgOri, (ShowW, ShowH))
        cvHist = cv2.calcHist([cvImgShow], [0], None, [256], [0.0,255.0])
        minVal, maxVal, minLoc, maxLoc = cv2.minMaxLoc(cvHist)
        cvImgHist = np.zeros([256, 256,3], np.uint8)
        hpt = int(0.9*256)
        for h in range(256):
            intensity = int(cvHist[h]*hpt/maxVal)
            cv2.line(cvImgHist, (h, 256), (h, 256-intensity), [0, 0, 255])
        cv2.imshow('Img', cvImgShow)
        cv2.imshow('ImgHist', cvImgHist)
        cv2.waitKey(100)

    cv2.destroyAllWindows()