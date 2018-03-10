To Run:
Various testFunctions are avaliable to try out in final_main.cpp.
I used the clion editor to run my code.
There are comments along each one describing what they do. Be sure that your output directory is configured properly or else it will not be able to write out some of the files. It will throw an error if this is the case.

To run the index.html file, you need to have installed the npm packaged "http-server" using:

1. npm install -g http-server
2. Then inside the directory, run http-server.
3. Navigate in Chrome to http://127.0.0.1:8080/
4. Type the filename of the photo you would like to mark
5. Mark the photo, changing from destroy and protect appropriately.
6. Copy and paste the output to final_main.cpp


How long did the assignment take?
I worked on this project for 24+ hours.

What I Did:
  *Created a gradient energy function that returns an energy map of the image
  *Function to create a cumulative energy map in both horizontal and vertical directions
  *Function to create a cumulative energy map in both horizontal and vertical directions with masking
  *Function to create a cumulative energy map in both horizontal and vertical directions with blocking

  *Finding vertical and horizontal seams
  *Drawing vertical and horizontal seams
  *Removing vertical and horizontal seams from an image
  *Adding vertical and horizontal seams to an image
  *Increasing the size of an image using vertical and horizontal seams
  *Removing objects from an image
  *Removing objects from an image while protecting other objects
  *Removing objects from an image and then restoring the image size by adding seams
  *Amplifying content by a factor
  *Automatic determination of optimal seam orientation for seam removal

  *HTML/JS app to mark objects for removal or protection

Potential issues with your solution and explanation of partial completion (for partial credit)
A very specific structure is needed for the output files. I write out a lot of the energyMaps and such.
There is a lot of intuition that is required when configuring the parameters for the different functions.

The HTML/JS app outputs the best objects when you drag from left to right.

Collaboration acknowledgment (you must write your own code)
None. I just used the resources that were listed in my project proposal.

Troublesome areas:
I had a lot of trouble creating the JS app to mark the areas. I ran into a lot of odd bugs like Cross Origin Requests being denied, unable to access the image data because a security feature, etc.

Adding seams was by far the hardest part of this assignment. I had trouble figuring out a way to penalize duplicating a seam that was already duplicated. I used a masking technique where I stored all the seams that I have added in another image. I then added this mask to the original image when calculating the gradient energy. This increased the cost of going across the seam.

Object removal was the second hardest part of this assignment. I similarly used a type of masking technique. In the code I call it "blocking." In this case I store the areas that I want to remove in a seperate image, block area. Whenever I calculate the energy map I set any area in the input image that lies within the block area to be a low value so that seams will want to go through it. I do this while calculating the cumulative energy map.

The difference between these two techniques is that with masking I modify the image so that the calculated non-cumulative energies reflect higher energies for the masked areas whereas with blocking, I manipulate the cumulative energy map after the non-cumulative energies are calculated.
