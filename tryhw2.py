from uwimg import *
im = load_image("data/dog.jpg")
f = make_box_filter(7)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-box7")

from uwimg import *
im = load_image("data/dog.jpg")
f = make_box_filter(7)
blur = convolve_image(im, f, 1)
thumb = nn_resize(blur, blur.h//7, blur.w//7)
save_image(thumb, "dogthumb")

from uwimg import *
im = load_image("data/dog.jpg")
f = make_sharpen_filter()
blur = convolve_image(im, f, 1)
# clamp_image(blur)
save_image(blur, "dog-sharpen")

from uwimg import *
im = load_image("data/dog.jpg")
f = make_highpass_filter()
blur = convolve_image(im, f, 0)
# clamp_image(blur)
save_image(blur, "dog-highpass")

from uwimg import *
im = load_image("data/dog.jpg")
f = make_emboss_filter()
blur = convolve_image(im, f, 1)
# clamp_image(blur)
save_image(blur, "dog-emboss")

from uwimg import *
im = load_image("data/dog.jpg")
f = make_gaussian_filter(2)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-gauss2")

from uwimg import *
im = load_image("data/dog.jpg")
f = make_gaussian_filter(2)
lfreq = convolve_image(im, f, 1)
hfreq = im - lfreq
reconstruct = lfreq + hfreq
save_image(lfreq, "low-frequency")
save_image(hfreq, "high-frequency")
save_image(reconstruct, "reconstruct")

from uwimg import *
im = load_image("data/dog.jpg")
res = sobel_image(im)
mag = res[0]
feature_normalize(mag)
save_image(mag, "magnitude")

from uwimg import *
im = load_image("data/dog.jpg")
res = colorize_sobel(im)
feature_normalize(res)
save_image(res, "colorize_sobel")