%
% Copyright 2018 <Admobilize>
% MATRIX Labs  [http://creator.matrix.one]
%
% MATRIX Creator HAL is like free software: you can redistribute
% it and/or modify it under the terms of the GNU General Public License
% as published by the Free Software Foundation, either version 3 of the
% License, or (at your option) any later version.

% This program is distributed in the hope that it will be useful, but
% WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
% General Public License for more details.

% You should have received a copy of the GNU General Public License along
% with this program.  If not, see <http://www.gnu.org/licenses/>.
%

FSup = [8000 12000 16000 22050 24000 32000 44100 48000 96000];
Fcs = [4000 5000 8000 11000 12000 16000 19000 19000 19000];

fid = fopen('coeff2.h','w');
fprintf(fid,'{');

for i = 1:length(FSup)
B=16;
L = 128;
Fs= FSup(i);
Fc1 = 200;
Fc2 = Fcs(i);

Wn_1= (Fc1/Fs)/2*pi;
Wn_2= (Fc2/Fs)/2*pi;
Wn = [Wn_1 Wn_2];

coeff = fir1(L,Wn,'bandpass');

hz = [zeros(1,length(coeff))];
for i = 1:length(coeff)
  if(coeff(i)>=0)
    hz(i) = round(coeff(i)*power(2,B-1)-1); %% Fixed point coefficients
  else
    hz(i) = round(coeff(i)*power(2,B-1)+1); %% Fixed point coefficients
  end
end

%% Generate coeff file
fprintf(fid,'{ %d , { \n',Fs);
fprintf(fid,'%d\n',hz(2));
j=0;
for i = numel(hz):-1:3
  j=j+1;
  fprintf(fid,',%d \n',hz(i));
end
fprintf(fid,'}},\n',hz(i));
end
fclose(fid);