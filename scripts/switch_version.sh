#! /bin/bash
# Function to switch versions of PHP binaries.

  to_wait=()
  version="${1:-7.4}"
  tools=(pear pecl php phar phar.phar php-cgi php-config phpize phpdbg)
  [ -e /usr/lib/cgi-bin/php"$version" ] && sudo update-alternatives --set php-cgi-bin /usr/lib/cgi-bin/php"$version" & to_wait+=($!)
  [ -e /usr/sbin/php-fpm"$version" ] && sudo update-alternatives --set php-fpm /usr/sbin/php-fpm"$version" & to_wait+=($!)
  [ -e /run/php/php"$version"-fpm.sock ] && sudo update-alternatives --set php-fpm.sock /run/php/php"$version"-fpm.sock & to_wait+=($!)

  for tool in "${tools[@]}"; do
    if [ -e "/usr/bin/$tool$version" ]; then
      sudo update-alternatives --set "$tool" /usr/bin/"$tool$version" &
      to_wait+=($!)
    fi
  done

  wait "${to_wait[@]}"
