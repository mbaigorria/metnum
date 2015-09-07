function Lch = CholFromBlocks(A)
    % Obtengo cholesky de una dimension menos, es decir, de A_{n-1}
    L = chol(A(1:size(A,1)-1,1:size(A,2)-1), 'lower');
    
    % Calculo los valores que faltan y armo el resultado. Hacer primero las cuentas en papel! Pregunta: Quien es el menor principal de (n-1)x(n-1)?
    ultFila = A(size(A,1),1:size(A,1)-1) * inv(L'); % Ultima fila de la L de Cholesky (Lch)
    
    % Elemento (n+1,n+1) de la L de Cholesky (Lch)
    ultElem = sqrt(A(size(A,1),size(A,1)) - ultFila*ultFila');
    Lch = [L zeros(size(L,1),1); ultFila ultElem];
    
    % Codigo para chequar que dio bien. Reconstruimos.
    Ach = Lch*Lch';
    for i = size(A,1)
        for j = size(A,2)
            if abs(A(i,j) - Ach(i,j)) > 0.000001 % Completar condicion.
                display(['original: ', num2str(A(i,j), '%.20f'), ' cholesky: ', num2str(Ach(i,j),'%.20f')]);
                error(['no iguales i:', num2str(i), ' j:', num2str(j)])
            end
        end
    end
end

