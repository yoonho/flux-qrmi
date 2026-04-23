FROM fluxrm/flux-sched:latest
USER root
RUN apt-get update
RUN apt-get install -y iproute2
RUN apt-get install -y net-tools
RUN apt-get install -y iputils-ping
# Remove after development stage!
COPY --chown=fluxuser:fluxuser .git /home/fluxuser/.git/
COPY --chown=fluxuser:fluxuser .ssh /home/fluxuser/.ssh/
USER fluxuser
