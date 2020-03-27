import json
import glob

pathRoot = "**/assets/"
pathSprites = "**/sprites/"
data = \
    {
        "assets":
            {
                "sprites":
                    {
                        "wall":
                            [
                                # {
                                #     "name": "wolf_bricks",
                                #     "path": "sprites\\wolf_bricks.png"
                                # }
                            ],
                        "entity":
                            [

                            ]
                    },
                "tile_types":
                    [
                        # {
                        #     "id": 0
                        #     "name": "brickWall",
                        #     "sprite_name": "wolfBlueBricks",
                        #     "traversable": False
                        # }
                    ]
            }
    }

tileTypeId = 0
for file in glob.iglob(pathRoot + pathSprites + 'wall/**/*.png', recursive=True):
    first = file.rfind('\\') + 1
    last = file.rfind('.png')
    name = file[first:last]
    data["assets"]["sprites"]["wall"].append({"name": name, "path": file})
    data["assets"]["tile_types"].append({
        "id": tileTypeId,
        "name": name + "_wall",
        "sprite_name": name,
        "traversable": False
    })
    tileTypeId += 1
    print(file)

for file in glob.iglob(pathRoot + pathSprites + 'entity/**/*.png', recursive=True):
    first = file.rfind('\\') + 1
    last = file.rfind('.png')
    name = file[first:last]
    data["assets"]["sprites"]["entity"].append({"name": name, "path": file})
    # data["assets"]["tile_types"].append({
    #     "id": id_,
    #     "name": name + "_wall",
    #     "sprite_name": name,
    #     "traversable": False
    # })
    # id_ += 1
    print(file)

with open('asset_info.json', 'w') as outfile:
    json.dump(data, outfile, indent=2)
