FROM registry.gitlab.com/h-doc/images/base:deb9


WORKDIR /app

RUN wget http://download.dre.vanderbilt.edu/previous_versions/ACE-5.8.3.tar.gz && \
   tar xvzf ACE-5.8.3.tar.gz && cd ACE_wrappers && \
   mkdir build && cd build && ../configure --disable-ssl && make install -j3

COPY . .
 
RUN cd /app && mkdir build && cd build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local/ -DWITH_WARNINGS=0  -Wno-dev && \
    make -j3 install && cd ../ && rm -rf *


