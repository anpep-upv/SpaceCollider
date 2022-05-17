FROM golang:1.14-buster AS easy-novnc-build
WORKDIR /src
RUN go mod init build && \
    go get github.com/geek1011/easy-novnc@v1.1.0 && \
    go build -o /bin/easy-novnc github.com/geek1011/easy-novnc

FROM debian:latest AS env

RUN echo 'deb http://deb.debian.org/debian testing main' >> /etc/apt/sources.list

RUN apt-get update -y && \
    apt-get install -y --no-install-recommends openbox tigervnc-standalone-server supervisor gosu && \
    rm -rf /var/lib/apt/lists && \
    mkdir -p /usr/share/desktop-directories

RUN apt-get update -y && \
    apt-get install -y --no-install-recommends lxterminal nano wget openssh-client rsync ca-certificates xdg-utils htop tar xzip gzip bzip2 zip unzip libfreeimage-dev libglu1-mesa libxi-dev libxmu-dev libglu1-mesa-dev libpulse-dev g++ make gcc && \
    rm -rf /var/lib/apt/lists

RUN apt-get update -y && \
    apt-get install -y --no-install-recommends freeglut3-dev && \
    rm -rf /var/lib/apt/lists

COPY --from=easy-novnc-build /bin/easy-novnc /usr/local/bin/
COPY docker_files/menu.xml /etc/xdg/openbox/
COPY docker_files/supervisord.conf /etc/
EXPOSE 8080

COPY . /app/
WORKDIR /app/
RUN make -j8

RUN mkdir /data

RUN groupadd --gid 1000 app && \
    useradd --home-dir /data --shell /bin/bash --uid 1000 --gid 1000 app

CMD ["sh", "-c", "chown app:app /data /dev/stdout && exec gosu app supervisord"]