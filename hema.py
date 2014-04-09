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
    print len(spikes)
    pylab.plot([spike[0] for spike in spikes],
               [spike[1] for spike in spikes], 'k.')
    pylab.xlabel('Time, ms')
    pylab.ylabel('# of neuron')
    pylab.title('Network raster activity')
    pylab.axis([0., pylab.ceil(max([spike[0] for spike in spikes])),
                -1, max([spike[1] for spike in spikes]) + 1])
    pylab.savefig(os.path.join(figure_dir, 'spikes.png'))


def plot_spike_histogram(spikes, bin=0.1):
    print 'Plotting activity histogram.'
    global figure_dir
    pylab.figure(get_free_figure_number())
    pylab.clf()
    pylab.hist([spike[0] for spike in spikes],
               pylab.ceil(max([spike[0] for spike in spikes])))
    pylab.xlim([0., pylab.ceil(max([spike[0] for spike in spikes]))])
    pylab.xlabel('Time, ms')
    pylab.ylabel('Portion of active neurons')
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


def main():
    global data_dir, figure_dir

    if not os.path.exists(figure_dir):
        os.makedirs(figure_dir)

    print 'Extracting spike data.'
    with open(os.path.join(data_dir, 'spikes.txt')) as f:
        spikes = [[float(j) for j in i.split()] for i in f.readlines()]
        for spike in spikes:
            spike[1] = int(spike[1])
    # print spikes

    print 'Extracting potential data.'
    potentials = []
    with open(os.path.join(data_dir, 'potentials.txt')) as f:
        while (f.read(1)):
            potentials.append([float(f.readline().split()[2]),
                              [float(i) for i in f.readline().split()]])
    # print potentials

    plot_spike_rastr(spikes)
    plot_spike_histogram(spikes)
    plot_neuron_potential(potentials, 0)
    plot_neuron_potentials(potentials)
    plot_average_neuron_potential(potentials)

    pylab.show()


if __name__ == '__main__':
    main()
