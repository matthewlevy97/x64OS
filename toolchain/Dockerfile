FROM alpine:3.6

ADD build-toolchain.sh /opt/build-toolchain.sh

RUN chmod +x /opt/build-toolchain.sh
RUN /opt/build-toolchain.sh

ENV PATH "/opt/toolchain:$PATH"
ENV OSDEV_ENV "true"
ENV BUILDROOT "/opt/"
WORKDIR /opt
