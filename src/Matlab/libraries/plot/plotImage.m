function plotImage(raw_matrix, reconstructed_image, plots)
% Note: If you want to do some post-processing of the raw matrix do it 
%       before calling this function.
% The "reconstructed_image" variable is used only for getting the image
% properties and is NOT used for plotting the "raw_matrix". This function
% plots whatever is there in "raw_matrix" as an EIDORS image.

shg;
subplot(plots(1));
displayImage(plots(1), raw_matrix, reconstructed_image);
title(plots(1), "Original");

subplot(plots(2));
displayImage(plots(2), zeros(size(raw_matrix)), reconstructed_image);
title(plots(2), "Zeros");

sgtitle("Max. Reading: " + num2str(round(abs(max(reconstructed_image.node_data)))) + "    Time: " + num2str(round(toc,1)));
drawnow;

end

function displayImage(ax, matrix, img)

greyscale_data = mk_mosaic(matrix, 0, [], 0);
colour_data = calc_colours(greyscale_data, img);

out_img = reshape(colour_data, size(greyscale_data));

image(ax, out_img);
showElectrodes(size(out_img,1));

axis image
axis off
axis equal
axis tight

end

function showElectrodes(mat_dim)

persistent x y;
if isempty(x)
    t = pi/2 + (0:2*pi/16:2*pi);
    x = mat_dim/2*(1 + sin(t));
    y = mat_dim/2*(1 + cos(t));
end

hold on;
plot(x,y,'r.','MarkerSize',16);
text(x(1),y(1),{'5'},'VerticalAlignment','middle','HorizontalAlignment', 'left');
text(x(5),y(5),{'1'},'VerticalAlignment','bottom','HorizontalAlignment', 'center');
text(x(9),y(9),{'13'},'VerticalAlignment','middle','HorizontalAlignment', 'right');
text(x(13),y(13),{'9'},'VerticalAlignment','top','HorizontalAlignment', 'center');
hold off;
end
