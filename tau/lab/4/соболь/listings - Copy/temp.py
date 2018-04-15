import numpy as np
from scipy import optimize as opt
from scipy.stats import gmean
from scipy import integrate
import control


def get_constrains():
    def constrain_k(arg):
        k_i, k_p = arg
        return k_i

    def constrain_T(arg):
        k_i, k_p = arg
        return k_p + 1

    # def constrain_first(arg):
    #     k_i, k_p = arg
    #     return 0.75 * T + 0.75 * k + 2

    def constrain_second(arg):
        k_i, k_p = arg
        return 1.5 - 0.75 * k_i + 1.5 * k_p

    return [constrain_k, constrain_T,
            # constrain_first,
            constrain_second]


def get_roots(k_i, k_p):
    coeffs = [1, 2, 0.75 * (1 + k_p), 0.75 * k_i]
    roots = np.roots(coeffs)
    return roots


def optimize_by_Omega():
    def get_omega(arg):
        k_i, k_p = arg
        roots = get_roots(k_i, k_p)
        return gmean(abs(roots))

    arg_0 = [3, 3]
    min_at = opt.fmin_cobyla(lambda arg: get_omega(arg) * (-1), arg_0, get_constrains(), rhoend=1e-3)
    print(min_at)


def optimize_by_stability():
    def get_stability(arg):
        k_i, k_p = arg
        roots = get_roots(k_i, k_p)
        min_root = min(abs(np.real(roots)))
        return min_root

    arg_0 = [5, 5]
    min_at = opt.fmin_cobyla(lambda arg: get_stability(arg) * (-1),
                             arg_0,
                             get_constrains(),
                             rhoend=0.001)

    print(min_at)
    print(get_roots(*min_at))


def get_oscillation(arg):
    k_i, k_p = arg

    def get_max(w):
        return 3 * np.sqrt(k_i ** 2 + k_p ** 2 * w ** 2) / np.sqrt(
            9 * k_i ** 2 - 48 * k_i * w ** 2 + 9 * k_p ** 2 * w ** 2 - 24 * k_p * w ** 4 + 18 * k_p * w ** 2 + 16 * w ** 6 + 40 * w ** 4 + 9 * w ** 2)

    w_0 = 100
    max_at = opt.fmin_cobyla(lambda w: -get_max(w), w_0,
                             [lambda w: w, ],
                             rhoend=0.01,
                             disp=0)
    cur_max = get_max(max_at)
    print("max for %f,%f is %f at w=%f" % (k_i, k_p, cur_max, max_at))
    return cur_max


def optimize_by_oscillation():
    arg_0 = [5, 5]  # [ 586.41883716 565.68948754]
    min_at = opt.fmin_cobyla(get_oscillation, arg_0,
                             get_constrains(),
                             maxfun=50,
                             rhoend=0.1)
    print(min_at)
    print(get_oscillation(min_at))


def get_sys(k_i, k_p):
    numerator = [0.75 * k_p, 0.75 * k_i]
    denum = [1, 2, 0.75 * (1 + k_p), 0.75 * k_i]
    return control.tf(numerator, denum)


def optimize_by_integral():
    def integrate_error(arg):
        k_i, k_p = arg
        sys = get_sys(k_i, k_p)
        t = np.linspace(0, 5)
        u = [1, ] * len(t)
        time, response, _ = control.forced_response(sys, t, u)
        error = response - u
        area = integrate.simps(np.power(error, 2), t)
        print("Integral for %f,%f is %f" % (k_i, k_p, area))
        return area

    arg_0 = [5, 5]
    min_at = opt.fmin_cobyla(integrate_error, arg_0,
                             get_constrains(),
                             maxfun=100,
                             rhoend=0.01)
    print(min_at)


# print('OMEGA')
# optimize_by_Omega()
# print('STABILITY')
# optimize_by_stability()
# print('OSCILLATION')
# optimize_by_oscillation()
# print('INTEGRAL')
# optimize_by_integral()
get_oscillation((0, 100))
