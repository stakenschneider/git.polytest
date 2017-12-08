function df = diffSysNegative(t, y)
    a0 = 0;
    a1 = 25;
    b0 = 25;
    b1 = 5;

    % t <= 0 => x'' + a1 * x' + a0 * x = 0
    % Y(2) = x' && Y(1) = x
    % Y'(1) = Y(2) && Y'(2) = - a1 * Y(2) - a0
    df = [y(2); - a1 * y(2) - a0];
end