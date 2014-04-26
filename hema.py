import pylab
import os
import numpy.fft
import random


data_dir = 'data'
figure_dir = 'plots'
free_figure_number = 1


def get_free_figure_number():
    global free_figure_number
    free_figure_number += 1
    return free_figure_number - 1


def plot_spike_rastr(spikes):
    print 'Plotting raster plot.'
    global figure_dir
    pylab.figure(get_free_figure_number())
    pylab.clf()
    pylab.plot([spike[0] for spike in spikes],
               [spike[1] for spike in spikes], 'k.')
    pylab.xlabel('Time, ms')
    pylab.ylabel('# of neuron')
    pylab.title('Network raster activity')
    pylab.axis([0., pylab.ceil(max([spike[0] for spike in spikes])),
                -1, max([spike[1] for spike in spikes]) + 1])
    pylab.savefig(os.path.join(figure_dir, 'spikes.png'))


def plot_spike_histogram(spikes, bin=0.1, total_neurons=None):
    print 'Plotting activity histogram.'
    global figure_dir
    pylab.figure(get_free_figure_number())
    pylab.clf()
    pylab.hist([spike[0] for spike in spikes],
               bins=pylab.ceil(max([spike[0] for spike in spikes])/bin))
    if total_neurons is not None:
        pylab.ylim([0, total_neurons])
    pylab.xlim([0., pylab.ceil(max([spike[0] for spike in spikes]))])
    pylab.xlabel('Time, ms')
    pylab.ylabel('Amount of active neurons')
    pylab.title('Average network activity')
    pylab.savefig(os.path.join(figure_dir, 'activity.png'))


def plot_neuron_potential(potentials, neuron_number=0):
    print 'Plotting neuron #%d potential.' % neuron_number
    global figure_dir
    pylab.figure(get_free_figure_number())
    pylab.clf()
    pylab.plot([i[0] for i in potentials],
               [i[1][neuron_number] for i in potentials])
    pylab.xlabel('Time, ms')
    pylab.ylabel('Potential, mV')
    pylab.title('Neuron #%d potential' % neuron_number)
    pylab.savefig(os.path.join(figure_dir,
                               'neuron_potential_%d.png' % neuron_number))


def plot_synaptic_current(synaptic_currents, neuron_number=0):
    print 'Plotting neuron #%d synaptic current.' % neuron_number
    global figure_dir
    pylab.figure(get_free_figure_number())
    pylab.clf()
    pylab.plot([i[0] for i in synaptic_currents],
               [i[1][neuron_number] for i in synaptic_currents])
    pylab.xlabel('Time, ms')
    pylab.ylabel('Synaptic current, pA')
    pylab.title('Neuron #%d synaptic_current' % neuron_number)
    pylab.savefig(os.path.join(figure_dir,
                               'synaptic_current_%d.png' % neuron_number))


def plot_neuron_potentials(potentials):
    print 'Plotting all neuron potentials.'
    global figure_dir
    pylab.figure(get_free_figure_number())
    pylab.clf()
    for j in xrange(len(potentials[0][1])):
        print '\tneuron #%d\r' % j,
        pylab.plot([i[0] for i in potentials],
                   [i[1][j] for i in potentials])
    print
    pylab.xlabel('Time, ms')
    pylab.ylabel('Potential, mV')
    pylab.title('Neuron potentials')
    pylab.savefig(os.path.join(figure_dir, 'neuron_potentials.png'))


def plot_synaptic_currents(synaptic_currents):
    print 'Plotting all neuron synaptic_currents.'
    global figure_dir
    pylab.figure(get_free_figure_number())
    pylab.clf()
    for j in xrange(len(synaptic_currents[0][1])):
        print '\tneuron #%d\r' % j,
        pylab.plot([i[0] for i in synaptic_currents],
                   [i[1][j] for i in synaptic_currents])
    print
    pylab.xlabel('Time, ms')
    pylab.ylabel('Synaptic current, pA')
    pylab.title('Synaptic currents')
    pylab.savefig(os.path.join(figure_dir, 'synaptic_currents.png'))


def plot_average_neuron_potential(potentials):
    print 'Plotting average neuron potential.'
    global figure_dir
    pylab.figure(get_free_figure_number())
    pylab.clf()
    pylab.plot([i[0] for i in potentials],
               [sum(i[1])/len(potentials[0][1]) for i in potentials])
    pylab.xlabel('Time, ms')
    pylab.ylabel('Potential, mV')
    pylab.title('Neuron average potential')
    pylab.savefig(os.path.join(figure_dir, 'neuron_average_potential.png'))


def plot_synaptic_weight_histogram(synaptic_weights, number=0, bins=10):
    print 'Plotting synaptic weights histogram, time = %f' % (
        synaptic_weights[number][0])
    global figure_dir
    pylab.figure(get_free_figure_number())
    pylab.clf()
    pylab.hist(synaptic_weights[number][1], bins=bins)
    pylab.xlabel('Weight value')
    pylab.ylabel('Amount of synapses with this weight')
    pylab.savefig(os.path.join(figure_dir, 'synaptic_weight_histogram.png'))


def plot_average_synaptic_weight(synaptic_weights):
    print 'Plotting average synaptic weight.'
    global figure_dir
    pylab.figure(get_free_figure_number())
    pylab.clf()
    pylab.plot([i[0] for i in synaptic_weights],
               [sum(i[1]) / len(synaptic_weights[0][1])
                for i in synaptic_weights])
    pylab.xlabel('Time, ms')
    pylab.ylabel('Average synaptic weight')
    pylab.title('Average synaptic weight')
    pylab.savefig(os.path.join(figure_dir, 'neuron_synaptic_weight.png'))


def extract_spike_data():
    print 'Extracting spike data.'
    with open(os.path.join(data_dir, 'spikes.txt')) as f:
        spikes = [[float(j) for j in i.split()] for i in f.readlines()]
        for spike in spikes:
            spike[1] = int(spike[1])
    # print spikes
    return spikes


def extract_potential_data():
    print 'Extracting potential data.'
    potentials = []
    with open(os.path.join(data_dir, 'potentials.txt')) as f:
        while (f.read(1)):
            potentials.append([float(f.readline().split()[2]),
                              [float(i) for i in f.readline().split()]])
    # print potentials
    return potentials


def extract_synaptic_currents():
    print 'Extracting synaptic currents.'
    synaptic_currents = []
    with open(os.path.join(data_dir, 'synaptic_currents.txt')) as f:
        while (f.read(1)):
            synaptic_currents.append([float(f.readline().split()[2]),
                                     [float(i) for i in f.readline().split()]])
    # print synaptic_currents
    return synaptic_currents


def extract_synaptic_weigths():
    print 'Extracting synaptic weight data.'
    synaptic_weights = []
    with open(os.path.join(data_dir, 'synaptic_weights.txt')) as f:
        while (f.read(1)):
            synaptic_weights.append([float(f.readline().split()[2]),
                                    [float(i) for i in f.readline().split()]])
    # print synaptic_weights
    return synaptic_weights


def extract_synaptic_data(total_synapses):
    print 'Extracting synaptic data.'
    synaptic_data = []
    with open(os.path.join(data_dir, 'synaptic_data.txt')) as f:
        while (f.read(1) and f.read(1)):
            synaptic_data.append([float(f.readline().split()[2]),
                                  [[float(i) for i in f.readline().split()]
                                   for _ in xrange(total_synapses)]])
    # print synaptic_data
    return synaptic_data


def plot_synapse_data(synaptic_data, synapse_number=0):
    print 'Plotting synapse #%d data.' % synapse_number
    global figure_dir
    pylab.figure(get_free_figure_number())
    pylab.clf()
    for j in xrange(len(synaptic_data[0][1])):
        pylab.plot([i[0] for i in synaptic_data],
                   [i[1][synapse_number][j] for i in synaptic_data])
    pylab.xlabel('Time, ms')
    pylab.title('Synapse %d data' % synapse_number)
    pylab.savefig(os.path.join(figure_dir,
                               'synapse_data_%d.png' % synapse_number))


def plot_average_synapse_data(synaptic_data, data_num, fignum=None):
    print 'Plotting average synaptic data, data number = %d.' % data_num
    global figure_dir
    if fignum is None:
        fignum = get_free_figure_number()
    pylab.figure(fignum)
    pylab.plot([i[0] for i in synaptic_data],
               [sum([stamp[1][i][data_num] for i in xrange(len(stamp[1]))])
                / len(stamp[1]) for stamp in synaptic_data])
    pylab.xlabel('Time, ms')
    pylab.title('Average synaptic data %d' % data_num)
    pylab.savefig(os.path.join(figure_dir,
                               'average_synaptic_data_%d.png' % data_num))
    return fignum


def calculate_activity_histogram(spikes, total_neurons, bin=0.1):
    """Calculates histogram and bins specifically for neurons.

    Bins are provided in seconds instead of milliseconds."""
    hist, bin_edges = pylab.histogram(
        [spike[0] for spike in spikes],
        bins=pylab.ceil(max([spike[0] for spike in spikes])/bin))
    bin_edges = pylab.delete(bin_edges, len(bin_edges)-1) / 1000.
    return [[float(i)/total_neurons for i in hist], bin_edges]


def calculate_activity_histogram_one_neu(spikes, bins):
    """Calculates histogram of one neuron"""
    hist, bin_edges = pylab.histogram(spikes, bins=bins)
    bin_edges = pylab.delete(bin_edges, len(bin_edges)-1) / 1000.
    return [[float(i) for i in hist], bin_edges]


def analyse_bursts_CUT(spikes, total_neurons):
    activity_hist, bins = calculate_activity_histogram(spikes, total_neurons,
                                                       bin=1.)
    activity_median = pylab.median(activity_hist)
    activity_mean = pylab.mean(activity_hist)
    pylab.figure(10)
    pylab.plot(bins, activity_hist, 'k')
    pylab.plot(bins, [activity_median for _ in bins], 'r')
    pylab.plot(bins, [activity_mean for _ in bins], 'b')

    FFT = abs(numpy.fft.rfft(activity_hist))
    freqs = numpy.fft.rfftfreq(len(activity_hist), bins[1] - bins[0])
    pylab.figure(11)
    fft = FFT
    for i in xrange(len(FFT)):
        if i < 20:
            fft[i] = sum(FFT[0:40])/40.
        elif i > len(FFT) - 21:
            fft[i] = sum(FFT[len(FFT) - 21:])/40.
        else:
            fft[i] = sum(FFT[i-20:i+20])/40.
    pylab.plot(freqs, fft)

    left = fft[1:len(fft)//2]
    right = fft[len(fft)//2:]
    # print fft
    # print left
    # print right

    left_max = max(left)
    left_max_pos = pylab.where(left == left_max)[0][0]
    right_max = max(right)
    right_max_pos = len(fft)//2 + pylab.where(right == right_max)[0][0]
    print left_max, freqs[left_max_pos]
    print right_max, freqs[right_max_pos]

    pylab.plot([freqs[left_max_pos], freqs[left_max_pos]],
               [0., left_max], 'r')
    pylab.plot([freqs[right_max_pos], freqs[right_max_pos]],
               [0., right_max], 'r')

    middle = fft[left_max_pos:right_max_pos]
    middle_min = min(middle)
    middle_min_pos = left_max_pos + pylab.where(middle == middle_min)[0][0]
    print middle_min, freqs[middle_min_pos]

    median = pylab.median(fft)
    median_pos = pylab.where(fft == median)[0][0]

    median2 = pylab.median(middle)
    minami = min(abs(middle-median2))
    if pylab.where(middle == median2 + minami)[0].size:
        median2_pos = (left_max_pos
                       + pylab.where(middle == median2 + minami)[0][0])
    else:
        median2_pos = (left_max_pos
                       + pylab.where(middle == median2 - minami)[0][0])
    print median2, fft[median2_pos]

    pylab.plot([freqs[middle_min_pos], freqs[middle_min_pos]],
               [0., middle_min], 'r')
    pylab.plot([freqs[median_pos], freqs[median_pos]],
               [0., median], 'k', label='median1')
    pylab.plot([freqs[median2_pos], freqs[median2_pos]],
               [0., median2], 'm', label='median2')
    pylab.legend()

    cutting_frequency = 10. / freqs[median_pos]
    print freqs[median_pos], cutting_frequency
    pylab.figure(10)
    pylab.plot(bins, [cutting_frequency for _ in bins], 'm')

    # print activity_median, activity_mean


def smooth_data(v, width):
    """Averages values at each index over some period"""
    wl = width // 2
    wr = width - wl
    ans_left = sum(v[0:width]) / width
    ans_right = sum(v[len(v) - width:]) / width
    ans = pylab.zeros(len(v))
    for i in xrange(wl):
        ans[i] = ans_left
    for i in xrange(wl, len(v) - wr + 1):
        ans[i] = sum(v[i - wl:i + wr]) / width
    for i in xrange(len(v) - wr + 1, len(v)):
        ans[i] = ans_right
    return ans


def analyse_bursts2_CUT(spikes, total_neurons, time_max):
    spikes_per_neuron = [[] for _ in xrange(total_neurons)]
    for spike in spikes:
        spikes_per_neuron[spike[1]].append(spike[0])

    # total_neurons = 1000
    analysing_neurons = sorted(random.sample(range(total_neurons), 1))
    print analysing_neurons

    # activity_hist_per_neuron = []
    # bins_per_neuron = []
    fft_per_neuron = []
    fft_sum = pylab.zeros(pylab.ceil(time_max*10.)/2+1)
    for i in analysing_neurons:
        a, b = calculate_activity_histogram_one_neu(
            spikes_per_neuron[i], pylab.ceil(time_max*10.))
        # activity_hist_per_neuron.append(a)
        # bins_per_neuron.append(b)
        # fft_per_neuron.append(abs(numpy.fft.rfft(a)))
        fft_sum += abs(numpy.fft.rfft(a))
        print 'Finished %f \r' % (float(i)/total_neurons),
    freqs_per_neuron = numpy.fft.rfftfreq(len(a), b[1] - b[0])
    print
    fft_sum /= len(analysing_neurons)

    # pylab.figure(10)
    # for i in analysing_neurons:
        # pylab.plot(bins_per_neuron[i], activity_hist_per_neuron[i])

    pylab.figure(11)
    # fft_sum = [sum([fft_per_neuron[neu][i]
                    # for neu in xrange(len(fft_per_neuron))])
               # for i in xrange(len(fft_per_neuron[0]))]
    # fft_sum[0] = fft_sum[1]
    fft_sum = smooth_data(fft_sum, 100)
    pylab.plot(freqs_per_neuron, fft_sum)


def index_approx(v, value):
    """Find index of element closest to value"""
    if isinstance(v, (list, tuple)):
        a = min([abs(i-value) for i in v])
        try:
            return v.index(value + a)
        except ValueError:
            return v.index(value - a)
    elif isinstance(v, pylab.ndarray):
        a = min(abs(v-value))
        if pylab.where(v == value + a)[0].size:
            return pylab.where(v == value + a)[0][0]
        else:
            return pylab.where(v == value - a)[0][0]


def ND(x, y):
    """Numerical Derivative with trash at ends"""
    answ = []
    for i in xrange(len(x)-1):
        answ.append((y[i+1] - y[i]) / (x[i+1] - x[i]))
    answ.append(answ[-1])
    return answ


def get_bursts(spikes, total_neurons, time_max, verbose=True):
    print 'Extracting bursts.'
    activity_hist, bins = calculate_activity_histogram(spikes, total_neurons,
                                                       bin=1.)
    # NB: BINS IN SECONDS!
    s_activity_hist = sorted(activity_hist, reverse=True)

    # Angle of touching line
    angle = pylab.pi * 3. / 4.
    s_activity_hist_deriv = smooth_data(ND(bins, s_activity_hist), 20)
    cutter = s_activity_hist[
        index_approx(s_activity_hist_deriv, pylab.tan(angle))]
    if verbose:
        print 'Burst threshold found at %f' % cutter

    s_activity_hist = [i if i > cutter else 0 for i in activity_hist]
    s_activity_hist = smooth_data(s_activity_hist, 20)

    bursts_indices_start = []
    bursts_indices_end = []

    for i in xrange(len(s_activity_hist)-1):
        if s_activity_hist[i+1] > 0 and s_activity_hist[i] == 0:
            bursts_indices_start.append(i+1)
        if s_activity_hist[i+1] == 0 and s_activity_hist[i] > 0:
            bursts_indices_end.append(i)

    if bins[bursts_indices_end[0]] < bins[bursts_indices_start[0]]:
        bursts_indices_end.pop(0)
    if bins[bursts_indices_end[-1]] < bins[bursts_indices_start[-1]]:
        bursts_indices_end.pop(-1)

    bursts_indices = []
    for i in xrange(len(bursts_indices_start)):
        activity_hist_buf = activity_hist[bursts_indices_start[i]:
                                          bursts_indices_end[i]]
        bursts_indices.append(
            bursts_indices_start[i] + activity_hist_buf.index(
                max(activity_hist_buf)))

    return (bins, activity_hist, [{'max': bursts_indices[i],
                                   'start': bursts_indices_start[i],
                                   'end': bursts_indices_end[i]}
                                  for i in xrange(len(bursts_indices))])


def plot_found_bursts(bins, activity_hist, bursts):
    print 'Plotting found bursts.'
    global figure_dir
    pylab.figure(get_free_figure_number())
    pylab.clf()
    pylab.plot(bins, activity_hist, label='Activity')
    for burst in bursts:
        pylab.plot([bins[burst['start']], bins[burst['start']]],
                   [0., activity_hist[burst['start']]], 'g')
        pylab.plot([bins[burst['max']], bins[burst['max']]],
                   [0., activity_hist[burst['max']]], 'r')
        pylab.plot([bins[burst['end']], bins[burst['end']]],
                   [0., activity_hist[burst['end']]], 'g')
    pylab.plot(0., 0., 'g', label='start')
    pylab.plot(0., 0., 'r', label='max')
    pylab.plot(0., 0., 'g', label='end')
    pylab.legend()
    pylab.xlabel('Time, s')
    pylab.ylabel('Portion of active neurons.')
    pylab.title('Activity and durst marks.')
    pylab.savefig(os.path.join(figure_dir, 'ibi_hist.png'))


def calculate_bursts_lengths(bins, bursts):
    lengths = []
    for burst in bursts:
        lengths.append(bins[burst['end']] - bins[burst['start']])
    return lengths


def calculate_IBIs(bins, bursts):
    # InterBurst Intervals
    IBIs = []
    for i in xrange(len(bursts)-1):
        IBIs.append(bins[bursts[i+1]['start']] - bins[bursts[i]['end']])
    return IBIs


def plot_IBIs_histogram(IBIs, bins=10):
    print 'Plotting IBI histogram.'
    global figure_dir
    pylab.figure(get_free_figure_number())
    pylab.clf()
    pylab.hist(IBIs, bins=bins)
    pylab.xlabel('InterBurst Interval, s')
    pylab.ylabel('Number of bursts')
    pylab.title('IBI histogram')
    pylab.savefig(os.path.join(figure_dir, 'ibi_hist.png'))


def main():
    global data_dir, figure_dir
    with open(os.path.join(data_dir, 'simulation.txt')) as f:
        total_neurons = int(f.readline().split()[1])
        total_synapses = int(f.readline().split()[1])
        time_max = float(f.readline().split()[1])
        dt = float(f.readline().split()[1])

    if not os.path.exists(figure_dir):
        os.makedirs(figure_dir)

    spikes = None
    potentials = None
    synaptic_weights = None
    synaptic_data = None
    synaptic_currents = None

    spikes = extract_spike_data()
    # plot_spike_rastr(spikes)
    # plot_spike_histogram(spikes, bin=10., total_neurons=total_neurons)

    # potentials = extract_potential_data()
    # plot_neuron_potential(potentials, 0)
    # plot_neuron_potentials(potentials)
    # plot_average_neuron_potential(potentials)

    # synaptic_weights = extract_synaptic_weigths()
    # plot_synaptic_weight_histogram(synaptic_weights)
    # plot_synaptic_weight_histogram(synaptic_weights,
    #                                number=len(synaptic_weights)-1)
    # plot_average_synaptic_weight(synaptic_weights)

    # synaptic_data = extract_synaptic_data(total_synapses)
    # plot_synapse_data(synaptic_data)
    # fn = plot_average_synapse_data(synaptic_data, 0)
    # plot_average_synapse_data(synaptic_data, 1, fignum=fn)

    # synaptic_currents = extract_synaptic_currents()
    # plot_synaptic_currents(synaptic_currents)
    # plot_synaptic_current(synaptic_currents, 1)

    bins, activity_hist, bursts = get_bursts(
        spikes, total_neurons, time_max)

    plot_found_bursts(bins, activity_hist, bursts)
    IBIs = calculate_IBIs(bins, bursts)
    plot_IBIs_histogram(IBIs, bins=5)

    if (isinstance(total_neurons, int) and isinstance(time_max, float)
            and spikes is not None):
        print 'Average frequency is %f Hz' % (
            float(len(spikes)) / float(total_neurons) / time_max * 1000.)

    pylab.show()


if __name__ == '__main__':
    main()
