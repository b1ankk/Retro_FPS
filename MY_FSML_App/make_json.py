import json
import glob


def find_asset_for_name(data_list, name):
    i = 0
    for field in data_list:
        if field["name"] == name:
            return i
        i += 1
    return -1


def find_anim_for_name(data_list, name):
    i = 0
    for field in data_list:
        if field["anim_name"] == name:
            return i
        i += 1
    return -1


def name_for_path(path, extension=''):
    dotIndex = extension.rfind('.')
    if dotIndex > 0 and extension != '':
        extension = extension[dotIndex:]
    elif dotIndex == -1 and extension != '':
        extension = '.' + extension

    first = path.rfind('\\') + 1
    last = path.rfind(extension)
    name = path[first:last]
    return name


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

                            ],
                        "ui":
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
                    ],
                "animations":
                    {
                        "enemy":
                            [
                                # {
                                #     "name": "frogmon",
                                #     "anims":
                                #         [
                                #             {
                                #                 "anim_name": "frogmon_walk",
                                #                 "frames":
                                #                     [
                                #                         {
                                #                             "frame_name": "frogmon_walk_00",
                                #                             "time": 0.4
                                #                         },
                                #                         {
                                #                             "frame_name": "frogmon_walk_01",
                                #                             "time": 0.8
                                #                         },
                                #                         {
                                #                             "frame_name": "frogmon_walk_02",
                                #                             "time": 1.2
                                #                         },
                                #                         {
                                #                             "frame_name": "frogmon_walk_03",
                                #                             "time": 1.6
                                #                         }
                                #                     ]
                                #             }
                                #
                                #         ]
                                # }
                            ],
                        "gun":
                            [

                            ]
                    },
                "fonts":
                    [
                        # "id": 0,
                        # "name": "pixeled",
                        # "path": "fonts\\Pixeled.tff"
                    ]
            }
    }

print("WALL SPRITES: ")
tileTypeId = 0
for file in glob.iglob(pathRoot + pathSprites + 'wall/**/*.png', recursive=True):
    name = name_for_path(file, 'png')
    data["assets"]["sprites"]["wall"].append({"name": name, "path": file})
    data["assets"]["tile_types"].append({
        "id": tileTypeId,
        "name": name + "_wall",
        "sprite_name": name,
        "traversable": False
    })
    tileTypeId += 1
    print(file)

print("ENTITY SPRITES: ")
for file in glob.iglob(pathRoot + pathSprites + 'entity/**/*.png', recursive=True):
    name = name_for_path(file, 'png')
    data["assets"]["sprites"]["entity"].append({"name": name, "path": file})
    print(file)

print("UI SPRITES: ")
for file in glob.iglob(pathRoot + pathSprites + 'ui/**/*.png', recursive=True):
    name = name_for_path(file, 'png')
    data["assets"]["sprites"]["ui"].append({"name": name, "path": file})
    print(file)

print("ENEMY ANIMATIONS: ")
for enemy_subfolder in glob.iglob(pathRoot + pathSprites + '/entity/enemy/*', recursive=True):
    print(enemy_subfolder)
    enemy_name = name_for_path(enemy_subfolder)
    enemy_index = find_asset_for_name(data["assets"]["animations"]["enemy"], enemy_name)
    if enemy_index == -1:
        data["assets"]["animations"]["enemy"].append({"name": enemy_name, "anims": []})
        enemy_index = len(data["assets"]["animations"]["enemy"]) - 1

    for anim_folder in glob.iglob(enemy_subfolder + '/*anim_*'):
        prefix = 'anim_'
        print(anim_folder)
        anim_name = name_for_path(anim_folder)
        anim_name = anim_name[anim_name.rfind(prefix) + len(prefix):]
        anim_index = find_anim_for_name(data["assets"]["animations"]["enemy"][enemy_index]["anims"], anim_name)
        if anim_index == -1:
            data["assets"]["animations"]["enemy"][enemy_index]["anims"].append({"anim_name": anim_name, "frames": []})
            anim_index = len(data["assets"]["animations"]["enemy"][enemy_index]["anims"]) - 1

        endFrameTime = 0.4
        for frame in glob.iglob(anim_folder + f'/%s_%s*.*' % (enemy_name, anim_name)):
            frame_name = name_for_path(frame, '.')
            data["assets"]["animations"]["enemy"][enemy_index]["anims"][anim_index]["frames"].append(
                {
                    "frame_name": frame_name,
                    "time": endFrameTime.__round__(2)
                }
            )
            endFrameTime += 0.4

print("GUN ANIMATIONS: ")
for gun_subfolder in glob.iglob(pathRoot + pathSprites + '/ui/gun/*', recursive=True):
    print(gun_subfolder)
    gun_name = name_for_path(gun_subfolder)
    gun_index = find_asset_for_name(data["assets"]["animations"]["gun"], gun_name)
    if gun_index == -1:
        data["assets"]["animations"]["gun"].append({"name": gun_name, "anims": []})
        gun_index = len(data["assets"]["animations"]["gun"]) - 1

    for anim_folder in glob.iglob(gun_subfolder + '/*anim_*'):
        prefix = 'anim_'
        print(anim_folder)
        anim_name = name_for_path(anim_folder)
        anim_name = anim_name[anim_name.rfind(prefix) + len(prefix):]
        anim_index = find_anim_for_name(data["assets"]["animations"]["gun"][gun_index]["anims"], anim_name)
        if anim_index == -1:
            data["assets"]["animations"]["gun"][gun_index]["anims"].append({"anim_name": anim_name, "frames": []})
            anim_index = len(data["assets"]["animations"]["gun"][gun_index]["anims"]) - 1

        endFrameTime = 0.4
        for frame in glob.iglob(anim_folder + f'/%s_%s*.*' % (gun_name, anim_name)):
            frame_name = name_for_path(frame, '.')
            data["assets"]["animations"]["gun"][gun_index]["anims"][anim_index]["frames"].append(
                {
                    "frame_name": frame_name,
                    "time": endFrameTime.__round__(2)
                }
            )
            endFrameTime += 0.4

print("FONTS: ")
fontId = 0
for file in glob.iglob(pathRoot + 'fonts/*.ttf', recursive=True):
    name = name_for_path(file, 'ttf')
    data["assets"]["fonts"].append(
        {
            "id": fontId,
            "name": name,
            "path": file
        }
    )
    fontId += 1
    print(file)

with open('asset_info.json', 'w') as outfile:
    json.dump(data, outfile, indent=2)
