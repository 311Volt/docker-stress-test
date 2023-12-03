FROM alpine:3.18.5

COPY scripts/install-prerequisites.sh /root/
RUN chmod +x /root/install-prerequisites.sh
RUN /root/install-prerequisites.sh

COPY bin /root/app

EXPOSE 20311

RUN ls /root/app
WORKDIR /root/app

CMD "/root/app/docker-stress-test"