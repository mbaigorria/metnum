function ret = CheckCondChol(A)

tic
ret = 0;
for i = 1:size(A) % Completar rango para i (help size)
	if det(A(1:i,1:i) > 0, % Completar condicion de corte. Retorna false (0).
        return;
	end
end
ret = 1;
toc