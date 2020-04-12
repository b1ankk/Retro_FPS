import glob
from PIL import Image

stepX, stepY = 64, 64

for file in glob.iglob('*.png'):

    imageName = file[0:file.find('.png')]

    image = Image.open(file)
    width, height = image.size

    counter = 0

    for x in range(0, width, stepX):
        for y in range(0, height, stepY):
            image.crop((x, y, x + stepX, y + stepY)).save(f'%s_%02d.png' % (imageName, counter), 'PNG')
            counter += 1
