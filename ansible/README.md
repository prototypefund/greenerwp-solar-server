# Ansible Playbooks

You may use these [Ansible](https://www.ansible.com/) playbooks to configure
your single board computer.

Currently, only the Orange Pi Zero LTS (256) with Armbian Buster (Debian 10) is
supported and tested.

## Howto

Install Ansible (tested with v2.8.3). On Debian: `apt install ansible`.

To run a playbook, call `ansible-playbook`. In the following example, the minify
playbook is run for the OrangePI with IP 192.168.0.2

```bash
ansible-playbook minify.yml -i '192.168.0.102,' -u root -k
```

If `apt-get update` does fail, you might have to remove the proposed-updates
entries in `/etc/apt/sources.list`.

## Playbooks

### minify.yml

The Desktop image contains xorg and other packages that are not needed for a
server. This playbook will remove these packages.

### wordpress.yml

Installs a WordPress environment. Put your WordPress files unter /var/www/html
