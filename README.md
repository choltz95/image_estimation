# image_estimation
image estimation using Richard Alsing's stochastic hill climbing algorithm + simulated annealing

DEP: gcc + make (build-essentials), freeglut3 + freeglut3-dev, libpng + libpng-dev, feh (until I can figure out how to save glut renders to disk)


	
COMPARING IMAGES
The easy way the Average difference in pixel values.
Slightly harder psnr. http://en.wikipedia.org/wiki/Peak_signal-to-noise_ratio
Harder again... ssim http://en.wikipedia.org/wiki/Structural_similarity
There isn't any real solution to automating similarity as a human sees it. Humans are tricky.
-----
http://en.wikipedia.org/wiki/Scale-invariant_feature_transfo...
-----
