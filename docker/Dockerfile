FROM ubuntu:18.04
MAINTAINER "keisuke.nakasuga@gmail.com"

ENV QT_VERSION v5.9.1
ENV QT_CREATOR_VERSION v4.3.1

RUN apt-get -y update && apt-get -y install \
    qtbase5-dev qt5-qmake qt5-default libxcb-keysyms1-dev qtbase5-private-dev \
	libxcb-xinerama0-dev build-essential cmake tmux \
	wget zip git emacs

RUN apt-get update && apt-get -y install openssh-server
RUN ( echo "#!/bin/bash"; \
      echo ""; \
      echo "service ssh start";) > /root/entrypoint.sh && \
    chmod +x /root/entrypoint.sh && \
    sed -i.bak 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config && \
    echo "PermitUserEnvironment yes" >> /etc/ssh/sshd_config && \
    ( echo ""; \
      echo "export PATH=/usr/local/nvidia/bin:/usr/local/cuda/bin:/opt/conda/bin:$PATH"; \
      echo "export LIBRARY_PATH=/usr/local/cuda/lib64/stubs:"; \
      echo "export LD_LIBRARY_PATH=/usr/local/nvidia/lib:/usr/local/nvidia/lib64"; \
    ) >> /root/.bashrc && \
    mkdir /root/.ssh && chmod 700 /root/.ssh && \
    ( echo "PATH=/usr/local/nvidia/bin:/usr/local/cuda/bin:/opt/conda/bin:$PATH"; \
      echo "LIBRARY_PATH=/usr/local/cuda/lib64/stubs:"; \
      echo "LD_LIBRARY_PATH=/usr/local/nvidia/lib:/usr/local/nvidia/lib64"; \
    ) >> /root/.ssh/environment
RUN echo 'root:root' | chpasswd
RUN sed -i -e 's/#PermitUserEnvironment yes/PermitUserEnvironment yes/' /etc/ssh/sshd_config \
    && sed -i -e 's/#PermitRootLogin yes/PermitRootLogin yes/' /etc/ssh/sshd_config \
    && sed -i -e 's/#PasswordAuthentication yes/PasswordAuthentication yes/' /etc/ssh/sshd_config