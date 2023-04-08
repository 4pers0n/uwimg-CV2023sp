from uwimg import *
im = load_image("data/dogsmall.jpg")
a = nn_resize(im, im.h*4, im.w*4)
save_image(a, "dog4x-nn")

from uwimg import *
im = load_image("data/dogsmall.jpg")
a = bilinear_resize(im, im.h*4, im.w*4)
save_image(a, "dog4x-bl")