%% Datos
clear;
close all;


% Se cargan las esperanzas, las varianzas y las covarianzas de las 3
% acciones
mu = [0.0427; 0.0015; 0.0285];
sigma = [0.1^2 0.0018 0.0011; 0.0018 0.1044^2 0.0026; 0.0011 0.0026 0.1411^2];


%% Por lagrange

Alag = [2*sigma ones(size(sigma,1),1); ones(1, size(sigma,2)) 0];
blag = [zeros(size(sigma,1),1); 1];
xlag = Alag\blag

%% Matrix Lagrange

auxmatlag = sigma\ones(size(sigma,2),1);	


%Resolver por jacobi y gauss-seidel
auxmatlag = jacobi(sigma,ones(size(sigma,2),1),ones(size(sigma,2),1));
auxmatlag = gausssei(sigma,ones(size(sigma,2),1),ones(size(sigma,2),1));

xmatlag = auxmatlag/(ones(1,size(sigma,1))*auxmatlag)
