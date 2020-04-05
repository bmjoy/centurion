import pandas as pd
import codecs

def GenerateTranslationsTableXml():
    try:
        df = pd.read_csv("translationTable.csv").set_index("stringName")

        languages = df.columns.values 
        files = []
        for lan in languages:
            f = codecs.open("translationTable_" + lan + ".xml", "w", "utf-8")
            f.write("<translationTable language=\"" + lan + "\">\n")
            files.append(f)

        for index, row in df.iterrows():
            s = "\t<entry stringName=\"" + index + "\" result=\""
            for k in range(len(languages)):
                s2 = s + row[k] + "\" />\n"
                files[k].write(s2) 
            
        for f in files:
            f.write('</translationTable>')
            f.close()
            
        finish = input('The operation has been done successfully! Press enter to close.')
        
    except Exception as e: 
        input(e)    

if __name__=="__main__":
    print('=== GenerateTranslationsTableXml ===\n\n')
    GenerateTranslationsTableXml()