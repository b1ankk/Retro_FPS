import json
import glob


def find_enemy_for_name(data_list, name):
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
                            ]
                    }

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

for file in glob.iglob(pathRoot + pathSprites + 'ui/**/*.png', recursive=True):
    first = file.rfind('\\') + 1
    last = file.rfind('.png')
    name = file[first:last]
    data["assets"]["sprites"]["ui"].append({"name": name, "path": file})
    # data["assets"]["tile_types"].append({
    #     "id": id_,
    #     "name": name + "_wall",
    #     "sprite_name": name,
    #     "traversable": False
    # })
    # id_ += 1
    print(file)

for enemy_subfolder in glob.iglob(pathRoot + pathSprites + '/entity/enemy/*', recursive=True):
    print(enemy_subfolder)
    enemy_name = name_for_path(enemy_subfolder)
    enemy_index = find_enemy_for_name(data["assets"]["animations"]["enemy"], enemy_name)
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

with open('asset_info.json', 'w') as outfile:
    json.dump(data, outfile, indent=2)
