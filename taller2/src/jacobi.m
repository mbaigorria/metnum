function [X] = jacobi(A,b,x0)
    xAct = x0;
    n = size(A,1);
    while(norm(A*xAct - b) > 0.000001)
		xAnt = xAct;
        for i = 1:n
            suma = 0;
            for j = 1:n
                if(j ~= i)
                    suma = suma + A(i,j) * xAnt(j);
                end
            end
            xAct(i) = (b(i) - suma) / A(i,i);
        end
    end
    X = xAct;
end
