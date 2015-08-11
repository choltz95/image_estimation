# image_estimation
image estimation using Richard Alsing's stochastic hill climbing algorithm + simulated annealing

DEP: gcc + make (build-essentials), freeglut3 + freeglut3-dev, libpng + libpng-dev, feh (until I can figure out how to save glut renders to disk)
	
RESOURCES FOR COMPARING IMAGES

 * The easy way the Average difference in pixel values.
 * Slightly harder psnr. http://en.wikipedia.org/wiki/Peak_signal-to-noise_ratio
 * Harder again... ssim http://en.wikipedia.org/wiki/Structural_similarity
 * http://en.wikipedia.org/wiki/Scale-invariant_feature_transfo...


Right now, image processing is done using libpng. Until I use glreadpixels() (which may even be less efficient), I take a screenshot of the active window with scrot and read in the image using libpng before
comparing the current generation to the target. 

Simulating annealing is implemented by initializing the probability of altering all three of a polygon's components (color, position, size) regardless of the resulting fitness score to be high at 50%, but to decrease with respect to a cooling factor proportional to the current number of generations.