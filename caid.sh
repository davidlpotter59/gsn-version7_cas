#!/bin/bash

cscomp -nowarn casidcard4.sd
cscomp -nowarn casidcard5.sd
cscomp -nowarn casidcard6.sd
cscomp -nowarn casidcard7.sd
cscomp -nowarn casidcard8.sd
cscomp -nowarn casidcard9.sd

scp casidcard4.sb linux01.ggund.com:/home/davep
scp casidcard5.sb linux01.ggund.com:/home/davep
scp casidcard6.sb linux01.ggund.com:/home/davep
scp casidcard7.sb linux01.ggund.com:/home/davep
scp casidcard8.sb linux01.ggund.com:/home/davep
scp casidcard9.sb linux01.ggund.com:/home/davep


