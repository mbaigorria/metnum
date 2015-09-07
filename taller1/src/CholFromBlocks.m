function Lch = CholFromBlocks(A)
    % Obtengo cholesky de una dimension menos, es decir, de A_{n-1}
    L = chol(A(1:size(A,1)-1,1:size(A,2)-1), 'lower');
    
    % Calculo los valores que faltan y armo el resultado. Hacer primero las cuentas en papel! Pregunta: Quien es el menor principal de (n-1)x(n-1)?
    ultFila = _________; % Ultima fila de la L de Cholesky (Lch)
    ultElem = _________; % Elemento (n+1,n+1) de la L de Cholesky (Lch)
    Lch = [L zeros(size(L,1),1); ultFila' ultElem];
    
    
    % Codigo para chequar que dio bien. Reconstruimos.
    Ach = Lch*Lch';
    for i = size(A,1)
        for j = size(A,2)
            if __________________ % Completar condicion.
                error('no iguales')
            end
        end
    end
end

