import os
from PIL import Image
from pathlib import Path

def makePalette(subImages): 
    palette = set()
    for i in range(len(subImages)): 
        width, height = subImages[i].size
        curImage = subImages[i].load()
        for w in range(width): 
            for h in range(height): 
                #print(tuple(k for k in curImage[h, w]))
                palette.add(tuple(k for k in curImage[h, w]))
    print(palette)
    palList = []
    for val in palette: 
        palList.append(val)
    return palList

def cutImages(img): 
    toReturnList = []
    width, height = img.size
    coef = width//8
    for h in range(coef): 
        top = h * 8
        bottom = (h + 1) * 8
        for w in range(coef):
            left = w * 8
            right = (w + 1) * 8
            #print((left, top, right, bottom))
            subImg = img.crop((left, top, right, bottom))
            toReturnList.append(subImg)
    #toReturnList[0].show()
    return toReturnList

#left, right, upper, lower inverted
def encodeTile(subImages, curPalette): 
    colorList = []
    for val in curPalette: 
        colorList.append(val)
    #get bit 0 
    #get bit 1
    encodedSubImages = []
    for i in range(len(subImages)): 
        width, height = subImages[i].size
        curImage = subImages[i].load()
        encodedSub = [ [0] * width for i in range(height)]
        for w in range(width): 
            w1 = width - 1 - w
            for h in range(height): 
                h1 = height - 1 - h
                curColor = tuple(k for k in curImage[h, w])
                for c in range(len(colorList)): 
                    if(curColor == colorList[c]): 
                        #index should be c
                        #change index into binary representation
                        curIndex = bin(c)[2:].zfill(2)
                        encodedSub[w1][h1] = curIndex
        encodedSubImages.append(encodedSub)
    # print(encodedSubImages)

    #encodedSubImage bit 1
    encodedBit0Images = []
    encodedBit1Images = []

    for image in encodedSubImages: 
        width, height = subImages[i].size
        encodedBit0Image = [ [0] * width for i in range(height)]
        encodedBit1Image = [ [0] * width for i in range(height)]
        for w in range(len(image)): 
            for h in range(len(image[w])):
                encodedBit0Image[w][h] = image[w][h][0]
                encodedBit1Image[w][h] = image[w][h][-1]
        encodedBit0Images.append(encodedBit0Image)
        encodedBit1Images.append(encodedBit1Image)
    #image encoded with [width][height]
    return [encodedBit0Images, encodedBit1Images]

def encodePal(palette): 
    encodedPalList = []
    for color in palette: 
        curPal = []
        for channel in color: 
            #print("{0:#0{1}x}".format(channel,4))
            curPal.append("{0:#0{1}x}".format(channel,4))
        encodedPalList.append(curPal)
    print(encodedPalList)
    return encodedPalList


def main(): 
    images = []
    path = Path(__file__).parent
    if(os.path.basename(path) != "spirites"):
        path = Path(__file__).parent / "spirites"
    print(path)
    filelist=os.listdir(path)
    imagePaths = []
    for file in filelist:
        if (file.endswith(".png")):
            print("this is is si ")
            print(os.path.join(path,file))
            file = os.path.join(path,file)
            imagePaths.append(file)
            image = Image.open(file)
            images.append(image)

    subImages = []
    palettes = []
    bit0Arrays = []
    bit1Arrays = []
    for image in images: 
        subImage = cutImages(image)
        palette = makePalette(subImage)
        subImages.append(subImage)
        palettes.append(palette)
        bit0Array, bit1Array = encodeTile(subImage, palette)
        bit0Arrays.append(bit0Array)
        bit1Arrays.append(bit1Array)

    #process bit0, bit1 string
    for i in range(len(images)): 
        array0PathName = imagePaths[i][:-4] + "0.txt"
        array1PathName = imagePaths[i][:-4] + "1.txt"
        print("hello hello")
        print(bit0Arrays[i][0])
        str0 = ''
        for j in range(len(bit0Arrays[i][0])):
            str0 += "0b"
            for num in bit0Arrays[i][0][j]:
                str0 += num
            str0 += ","
        str1 = ''
        for j in range(len(bit1Arrays[i][0])):
            str1 += "0b"
            for num in bit1Arrays[i][0][j]:
                str1 += num
            str1 += ","
        f = open(array0PathName, "w")
        f.write(str0)
        f.close()
        f = open(array1PathName, "w")
        f.write(str1)
        f.close()

    #process palettes
    encodedPals = []
    for i in range(len(images)):
        encodedPal = encodePal(palettes[i])
        print(encodedPal)
        encodedPals.append(encodedPal)
        arrayPathName = imagePaths[i][:-4] + "pal.txt"
        str0 = ""
        encodedPal1 = []
        for k in range(len(encodedPal)): 
            if(k == 1):
                encodedPal1.append(encodedPal[2])
            elif(k ==2):
                encodedPal1.append(encodedPal[1])
            else: 
                encodedPal1.append(encodedPal[k])
        for j in range(len(encodedPal1)):
            j1 = len(encodedPal) - 1 - j
            curColor = encodedPal1[j]
            for k in curColor: 
                str0 += k + ","
            str0 += ";"
        f = open(arrayPathName,"w")
        f.write(str0)
        f.close()

main()
