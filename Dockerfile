FROM fluxrm/flux-sched:latest
USER root
RUN apt-get update
RUN apt-get install -y iproute2
RUN apt-get install -y net-tools
RUN apt-get install -y iputils-ping
RUN apt-get install -y less
RUN apt-get install -y software-properties-common
RUN add-apt-repository -y ppa:deadsnakes/ppa
RUN apt-get install -y python3.12 python3.12-venv python3.12-dev
RUN apt-get install -y python3-pip
RUN pip install --upgrade pip
RUN rm /usr/bin/python3
RUN ln -s python3.12 /usr/bin/python3
# Remove after development stage!
COPY --chown=fluxuser:fluxuser .git /home/fluxuser/.git/
COPY --chown=fluxuser:fluxuser .ssh /home/fluxuser/.ssh/
ENV MANPAGER="less -is"
USER fluxuser
