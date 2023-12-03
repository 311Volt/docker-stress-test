FROM alpine:3.18.5

WORKDIR /root

COPY scripts/install-prerequisites.sh /root/
RUN chmod +x /root/install-prerequisites.sh
RUN /root/install-prerequisites.sh

COPY scripts/install-crow.sh /root/
RUN chmod +x /root/install-crow.sh
RUN /root/install-crow.sh

COPY scripts/build-app.sh /root/
RUN mkdir /root/workdir

CMD ["sh", "/root/build-app.sh", "/root/workdir"]