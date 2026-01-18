ARG IMAGE=registry.gitlab.com/dev-hood/elpida/elpida/build-environment-base:libs-local
FROM $IMAGE
LABEL authors="klapeto"

ARG TARGET_TRIPLE

# help qt tools because they are built against the sysroot
ENV LD_LIBRARY_PATH="/opt/sysroots/x86_64-linux-gnu/lib:/opt/sysroots/x86_64-linux-gnu/lib/x86_64-linux-gnu"
ENV DEBIAN_FRONTEND=noninteractive

WORKDIR /tmp/
ADD ./scripts/docker/packages ./scripts/docker/generate-sysroot.sh ./scripts/docker/build-base.sh ./scripts/docker/build-qt.sh ./scripts/docker/build-hwloc.sh ./scripts/docker/build-llvm.sh  ./

WORKDIR /tmp/llvm-mingw
RUN TARGET_TRIPLES="$TARGET_TRIPLE" ./build-all.sh /opt/toolchains/llvm --with-default-msvcrt=msvcrt

#RUN apt-get -y install make
#
WORKDIR /tmp/
RUN DEB_DISTRIBUTION=focal ./generate-sysroot.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE" /tmp/packages
RUN ./build-hwloc.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE"
RUN ./build-qt.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE"
RUN ./build-llvm.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE"

WORKDIR /
RUN rm -rf /tmp/* \
    "/opt/sysroots/$TARGET_TRIPLE/tmp" \
    "/opt/sysroots/$TARGET_TRIPLE/var" \
    "/opt/sysroots/$TARGET_TRIPLE/usr/share" \
    "/opt/sysroots/$TARGET_TRIPLE/usr/sbom" \
    "/opt/sysroots/$TARGET_TRIPLE/usr/doc" \
    "/opt/sysroots/$TARGET_TRIPLE/usr/metatypes"

RUN if [ "$TARGET_TRIPLE" != $(gcc -dumpmachine) ]; then \
    rm -rf "/opt/sysroots/$TARGET_TRIPLE/bin" \
    "/opt/sysroots/$TARGET_TRIPLE/sbin" \
    "/opt/sysroots/$TARGET_TRIPLE/usr/bin" \
    "/opt/sysroots/$TARGET_TRIPLE/usr/sbin"; \
    fi

# cleanup
RUN apt-get -y purge g++ gcc libc6-dev automake autoconf libtool-bin
RUN apt-get -y autoremove && apt-get autoclean

CMD ["/bin/bash"]