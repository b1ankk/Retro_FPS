import glob
from PIL import Image

stepX, stepY = 18, 18

for file in glob.iglob('*.png'):

    imageName = file[0:file.find('.png')]

    image = Image.open(file)
    width, height = image.size

    counter = 0

    for x in range(2, width, stepX):
        for y in range(1, height, stepY):
            image.crop((x, y, x + stepX, y + stepY)).save(f'%s_%02d.png' % (imageName, counter), 'PNG')
            counter += 1
