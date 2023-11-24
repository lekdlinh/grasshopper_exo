a = 37;
grama = pi/9 %(pi*5)/18 pi/6
r = 28;
k = 24.8;
x = linspace(pi*2/3,0); %(pi*5)/9,100);%A = cos( pi/2 - acos(((2*r*r) - (2*a*r*cos(grama + x )))/(2*r*sqrt(a*a+r*r - 2*a*r*cos(grama + x)))));

for i = 1:length(x)
b = sin(acos(((2*r*r) - (2*a*r*cos(grama + x )))/(2*r*sqrt(a*a+r*r - 2*a*r*cos(grama + x)))));


y = ((sqrt(a*a + r*r - 2*a*r*cos(grama+x))-sqrt(a*a + r*r - 2*a*r*cos(grama))) * k * A)*0.028;
end
plot((x*360)/(2*pi),y);
disp(x);
disp(y);

   
fileID = fopen('values.txt', 'w');
fprintf(fileID, 'Gia tri cua x:\n');
fprintf(fileID, '%f\n', (x*360)/(2*pi));
fprintf(fileID, '\nGia tri cua y:\n');
fprintf(fileID, '%f\n', y);
fclose(fileID);