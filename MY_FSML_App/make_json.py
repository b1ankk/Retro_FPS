import json
import os
import re
import glob

data = {"assets":
    {
        "sprites":
            [
                # {
                #     "name": "wolf_bricks",
                #     "path": "sprites\\wolf_bricks.png"
                # }
            ],
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

id_ = 1
for file in glob.iglob('**/sprites/**/*.png', recursive=True):
    first = file.rfind('\\') + 1
    last = file.rfind('.png')
    name = file[first:last]
    data["assets"]["sprites"].append({"name": name, "path": file})
    data["assets"]["tile_types"].append({
        "id": id_,
        "name": name + "_wall",
        "sprite_name": name,
        "traversable": False
    })
    id_ += 1
    print(file)

with open('asset_info.json', 'w') as outfile:
    json.dump(data, outfile)
