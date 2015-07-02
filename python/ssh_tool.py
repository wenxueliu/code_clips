#! /usr/bin/env python
# encoding: utf-8


import os,sys,pexpect
class sshCopy:
    def __init__(self, user, passwd):
        self.user = user
        self.passwd = passwd

    ##TODO check the pub_key_path, remote_ip
    def ssh_copy_id(self, remote_ip, pub_key_path):
        """
        The authenticity of host 'app.broom.cm4 (172.24.102.132)' can't be established.
        DSA key fingerprint is 93:d0:b7:1a:09:ce:e8:c6:bb:b3:aa:17:b0:13:45:91.
        Are you sure you want to continue connecting (yes/no)?
        # ssh-copy-id -i ~/.ssh/id_rsa.pub xenconsole.cm5
        Warning: Permanently added 'xenconsole.cm5,172.25.89.230' (DSA) to the list of known hosts.
        root@xenconsole.cm5's password:

        """
        #print 'begin: /usr/bin/ssh-copy-id -i %s %s@%s' %(pub_key_path,self.user,remote_ip)
        child = pexpect.spawn('/usr/bin/ssh-copy-id -i %s %s@%s'
                            %( pub_key_path, self.user, remote_ip) )
        try:
            index = child.expect(['continue connecting \(yes/no\)', '\'s password:'],timeout=8)
            print "index:", index
            if index == 0:
                child.sendline('yes')
                index = child.expect(['continue connecting \(yes/no\)','\'s password:'],timeout=8)
                print "index",index
                child.sendline(passwd)
                child.expect('password:')
                child.sendline(passwd)
                #print "child.after:",child.after,"\nchild.before:",child.before
            if index == 1:
                child.sendline(passwd)
                child.expect('password:')
                child.sendline(passwd)
                print "success"
                #print "child.after:",child.after,"\nchild.before:",child.before
        except pexpect.TIMEOUT:
            print 'pexpect.TIMEOUT'
            #print "child.after:",child.after,"\nchild.before:",child.before
        except pexpect.EOF:
            print 'May ssh has been install in %s' %remote_ip
            #print "child.after:",child.after,"\nchild.before:",child.before
        else:
            print 'copy ssh key to ', hostname, " successfully" 
        finally:
            child.close()

if __name__=="__main__":

    current_user = "vagrant" #os.getenv('USER')
    passwd = 'vagrant'

    home_path = os.getenv('HOME')
    pub_key_path = home_path + '/.ssh/id_rsa.pub'
    file_cmd = 'file ' + pub_key_path

    key_exist = os.system(file_cmd)
    if key_exist == 0:
        hosts = ["10.1.2.11","10.1.2.12","10.1.2.13","10.1.2.14"]
        sshTool = sshCopy(current_user, passwd)
        [sshTool.ssh_copy_id(h,pub_key_path) for h in hosts]
    else:
        print "pub_key_path isn't exist"
