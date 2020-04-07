FROM alpine:3.6

ADD build-toolchain.sh /opt/build-toolchain.sh

ENV PATH "/opt/toolchain:$PATH"
ENV OSDEV_ENV "true"
ENV BUILDROOT "/opt/"
ENV SYSROOT "${BUILDROOT}/sysroot"
ENV ISO "${BUILDROOT}/kernel.iso"

RUN chmod +x /opt/build-toolchain.sh
RUN /opt/build-toolchain.sh

WORKDIR /opt
