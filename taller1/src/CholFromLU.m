function Lch = CholFromLU(A)
    % Conseguir factorizacion LU de A (help lu)
    [L,U,P] = lu(A);
    
    % Conseguir L de cholesky a partir de LU
    Lch = L*sqrt(U*inv(L')); % Quien es la L de Cholesky?    
    
    % Codigo para chequar que dio bien
    Ach = Lch*Lch'; % Reconstruimos.
    for i = size(A,1)
        for j = size(A,2)
            if abs(A(i,j) - Ach(i,j)) > 0.000001 % Comparamos A y Ach. Como?
                error('no iguales')
            end
        end
    end
end

