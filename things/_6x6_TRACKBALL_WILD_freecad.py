
import FreeCAD
import ImportGui
import Mesh

def closeDocs():
    documentList = App.listDocuments()

    for doc in documentList:
        App.closeDocument(doc)

def process_item(filename):
    closeDocs()
    ImportGui.open(filename + '.step')
    Gui.Selection.addSelection("Unnamed", 'Solid')
    obj = Gui.Selection.getSelection()
    Mesh.export(obj, filename + '_freecad.stl')

files = [ "/home/pascal/dev/dactyl-keyboard/things/_6x6_TRACKBALL_WILD_right","/home/pascal/dev/dactyl-keyboard/things/_6x6_TRACKBALL_WILD_left","/home/pascal/dev/dactyl-keyboard/things/_6x6_TRACKBALL_WILD_right_plate","/home/pascal/dev/dactyl-keyboard/things/_6x6_TRACKBALL_WILD_left_plate" ]

documentList = App.listDocuments()

for doc in documentList:
    App.closeDocument(doc)


for file in files:
    try:
        process_item(file)
    except Exception as err:
        print(f"Error {err=}, {type(err)=}")


print('Done')
