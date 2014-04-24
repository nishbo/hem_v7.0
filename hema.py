import pylab
import os


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
    hist, bin_edges = pylab.histogram(
        [spike[0] for spike in spikes],
        bins=pylab.ceil(max([spike[0] for spike in spikes])/bin))
    bin_edges = pylab.delete(bin_edges, len(bin_edges)-1) / 1000.
    return [[float(i)/total_neurons for i in hist], bin_edges]


def analyse_bursts(spikes, total_neurons):
    activity_hist, bins = calculate_activity_histogram(spikes, total_neurons,
                                                       bin=0.1)
    activity_median = pylab.median(activity_hist)
    activity_mean = pylab.mean(activity_hist)
    print activity_median, activity_mean
    pylab.figure(10)
    pylab.plot(bins, activity_hist, 'k')
    pylab.plot(bins, [activity_median for _ in bins], 'r')
    pylab.plot(bins, [activity_mean for _ in bins], 'b')

    FFT = abs(pylab.rfft(activity_hist))
    freqs = pylab.rfftfreq(len(activity_hist), bins[1] - bins[0])
    pylab.figure(11)
    for i in xrange(len(FFT)):
        if i < 20:
            FFT[i] = sum(FFT[0:40])/40.
        elif i > len(FFT) - 21:
            FFT[i] = sum(FFT[len(FFT) - 21:])/40.
        else:
            FFT[i] = sum(FFT[i-20:i+20])/40.
    pylab.plot(freqs, FFT)


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
    plot_spike_histogram(spikes, bin=10., total_neurons=total_neurons)

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

    analyse_bursts(spikes, total_neurons)

    if (isinstance(total_neurons, int) and isinstance(time_max, float)
            and spikes is not None):
        print 'Average frequency is %f Hz' % (
            float(len(spikes)) / float(total_neurons) / time_max * 1000.)

    pylab.show()


if __name__ == '__main__':
    main()
