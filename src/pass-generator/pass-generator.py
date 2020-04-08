from PIL import Image
import numpy as np
import os

GRID_CELL_SIZE = 20

def passCreate():
    try:
        lista_nomi = os.listdir()
        if lista_nomi:
            
            for filePath in lista_nomi:

                name = os.path.splitext(filePath)[0]
                extension = os.path.splitext(filePath)[1]

                if (extension == ".png"):
                
                    img = Image.open(filePath, 'r')
                    data = np.array(img)
                    data = np.array(img.split()[-1])
                    width = data.shape[1]
                    height = data.shape[0]
                    
                    xSize = width//GRID_CELL_SIZE//4 * 4 + 4 * int(width//GRID_CELL_SIZE%4 != 0)
                    ySize = height//GRID_CELL_SIZE//4 * 4 + 4 * int(height//GRID_CELL_SIZE%4 != 0)
                                       
                    out_data = []
                    for iy in range(ySize): 
                        l1 = []
                        for ix in range(xSize):
                            l1 += [0]
                        out_data += [l1]
                        
                    for iy in range(round(height/GRID_CELL_SIZE)): 
                        for ix in range(round(width/GRID_CELL_SIZE)):
                            value = int(data[iy*GRID_CELL_SIZE, ix*GRID_CELL_SIZE]/255)                            
                            if (value == 1):
                                out_data[iy][ix] = 1
                    
                    f = open(name+".xml", "w")
                    
                    f.write("<pass>\n")
                    f.write("\t<xSize>" + str(xSize) + "</xSize>\n")
                    f.write("\t<ySize>" + str(ySize) + "</ySize>\n")
                    f.write("\t<passGrid>")
                    
                    fourBinaries = ""
                    for iy in range(ySize):
                        for ix in range(xSize):
                            
                            value = out_data[iy][ix]
                                                                                                              
                            if len(fourBinaries) != 4:
                                fourBinaries += str(value)
                                                        
                            if len(fourBinaries) == 4:
                                hstr = '%0*X' % (1, int(fourBinaries, 2))
                                fourBinaries = ""
                                f.write(hstr)
                    
                    f.write("</passGrid>\n")
                    f.write("</pass>")
                    f.close()
    
    except Exception as e: 
        input(e)

if __name__=="__main__":
    passCreate()
    input('Press Enter to close.')