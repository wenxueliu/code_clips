#!/usr/bin/env python
# encoding: utf-8

from mininet.net import Mininet
from mininet.node import Controller, RemoteController, OVSSwitch
from mininet.cli import CLI
from mininet.log import setLogLevel, info
from mininet.link import Link, Intf, TCLink
from mininet.topo import Topo
from mininet.util import dumpNodeConnections
import logging
import os

logging.basicConfig(filename='./fattree.log', level=logging.DEBUG)
logger = logging.getLogger(__name__)


class Fattree(Topo):
    logger.debug("Class Fattree")
    CoreSwitchList = []
    AggSwitchList = []
    EdgeSwitchList = []
    HostList = []

    def __init__(self, k, density):
        logger.debug("Class Fattree init")
        self.pod = k
        #self.iCoreLayerSwitch = (k/2)**2
        #self.iAggLayerSwitch = k*k/2
        #self.iEdgeLayerSwitch = k*k/2
        self.iCoreLayerSwitch = k
        self.iAggLayerSwitch = k*2
        self.iEdgeLayerSwitch = k*2
        self.density = density
        self.iHost = self.iEdgeLayerSwitch * density
        self.dpid = 0
        self.switchClass = OVSSwitch

        #Init Topo
        Topo.__init__(self)

    def createTopo(self):
        self.createCoreLayerSwitch(self.iCoreLayerSwitch)
        self.createAggLayerSwitch(self.iAggLayerSwitch)
        self.createEdgeLayerSwitch(self.iEdgeLayerSwitch)
        #self.createHost(self.iHost)

    def dpidFrom(self, num):
        "Compute default dpid from number"
        fmt = ( '%0' + str( self.switchClass.dpidLen ) + 'x' )
        return fmt % num

    """
    Create Switch and Host
    """

    def _addSwitch(self, number, level, switch_list):
        for index in xrange(1, number+1):
            #if self.dpid < 16:
            #    dpid = "00:00:00:00:00:00:00:00:0" + str(hex(self.dpid))
            #elif self.dpid < 256:
            #    dpid = "00:00:00:00:00:00:00:00:" + str(hex(self.dpid))
            #else:
            #    print "dpid isn't enough, fix it"
            self.dpid += 1
            switch_list.append(self.addSwitch(level + str(index), dpid=self.dpidFrom(self.dpid)))
        #for x in xrange(1, number+1):
        #    PREFIX = str(level) + "00"
        #    if x >= int(10):
        #        PREFIX = str(level) + "0"
        #    switch_list.append(self.addSwitch('s' + PREFIX + str(x)))

    def createCoreLayerSwitch(self, NUMBER):
        logger.debug("Create Core Layer")
        self._addSwitch(NUMBER, "core", self.CoreSwitchList)

    def createAggLayerSwitch(self, NUMBER):
        logger.debug("Create Agg Layer")
        self._addSwitch(NUMBER, "agg", self.AggSwitchList)

    def createEdgeLayerSwitch(self, NUMBER):
        logger.debug("Create Edge Layer")
        self._addSwitch(NUMBER, "tor", self.EdgeSwitchList)

    def createHost(self, NUMBER):
        logger.debug("Create Host")
        for index in xrange(1, NUMBER+1):
            PREFIX="h"
            self.HostList.append(self.addHost(PREFIX + str(index)))

    """
    Add Link
    """
    def createLink(self, bw_c2a=0.2, bw_a2e=0.1, bw_h2a=0.5):
        logger.debug("Add link Agg to Core.")
        #end = self.pod/2
        for i in xrange(0, self.iCoreLayerSwitch):
            for j in xrange(0, self.iAggLayerSwitch):
                    self.addLink(
                        self.CoreSwitchList[i],
                        self.AggSwitchList[j],
                        bw=bw_c2a)

        logger.debug("Add link Edge to Agg.")
        for i in xrange(0, self.iAggLayerSwitch):
            edge_begin = i/self.pod * self.pod
            for j in xrange(edge_begin, edge_begin+self.pod):
                    self.addLink(
                        self.AggSwitchList[i],
                        self.EdgeSwitchList[j],
                        bw=bw_a2e)

        #logger.debug("Add link Edge to Host.")
        #for i in xrange(0, self.iEdgeLayerSwitch):
        #    host_begin = i * self.density
        #    for j in xrange(host_begin, host_begin+self.density):
        #        self.addLink(
        #            self.EdgeSwitchList[i],
        #            self.HostList[j],
        #            bw=bw_h2a)

    def set_ovs_protocol_13(self,):
        self._set_ovs_protocol_13(self.CoreSwitchList)
        self._set_ovs_protocol_13(self.AggSwitchList)
        self._set_ovs_protocol_13(self.EdgeSwitchList)

    def _set_ovs_protocol_13(self, sw_list):
            for sw in sw_list:
                cmd = "sudo ovs-vsctl set bridge %s protocols=OpenFlow13" % sw
                os.system(cmd)


def iperfTest(net, topo):
    logger.debug("Start iperfTEST")
    h1000, h1015, h1016 = net.get(
        topo.HostList[0], topo.HostList[14], topo.HostList[15])

    #iperf Server
    h1000.popen(
        'iperf -s -u -i 1 > iperf_server_differentPod_result', shell=True)

    #iperf Server
    h1015.popen(
        'iperf -s -u -i 1 > iperf_server_samePod_result', shell=True)

    #iperf Client
    h1016.cmdPrint('iperf -c ' + h1000.IP() + ' -u -t 10 -i 1 -b 100m')
    h1016.cmdPrint('iperf -c ' + h1015.IP() + ' -u -t 10 -i 1 -b 100m')


def pingTest(net):
    logger.debug("Start Test all network")
    net.pingAll()


def createTopo(pod, density, ip="127.0.0.1", port=6653, bw_c2a=0.2, bw_a2e=0.1, bw_h2a=0.05):
    logging.debug("LV1 Create Fattree")
    topo = Fattree(pod, density)
    topo.createTopo()
    topo.createLink(bw_c2a=bw_c2a, bw_a2e=bw_a2e, bw_h2a=bw_h2a)

    logging.debug("LV1 Start Mininet")
    CONTROLLER_IP = ip
    CONTROLLER_PORT = port
    net = Mininet(topo=topo, link=TCLink, controller=None, autoSetMacs=True,
                  autoStaticArp=True)
    net.addController(
        'controller', controller=RemoteController,
        ip=CONTROLLER_IP, port=CONTROLLER_PORT)
    net.start()

    '''
        Set OVS's protocol as OF13
    '''
    topo.set_ovs_protocol_13()

    logger.debug("LV1 dumpNode")

    #dumpNodeConnections(net.hosts)
    #pingTest(net)
    #iperfTest(net, topo)

    CLI(net)
    net.stop()

if __name__ == '__main__':
    setLogLevel('info')
    if os.getuid() != 0:
        logger.debug("You are NOT root")
    elif os.getuid() == 0:
        createTopo(2, 2)
