package org.wenxueliu.concurrent;

/** 
 * The class representing IPv4 address
 * @author Liu Wenxue
 */

@SuppressWarnings("all") public class IPv4Address {
    public static final int IP_ADDRESS_LENGTH = 4;
    private int address;

    public IPv4Address(int address){
        if (isVaild(address)){
            this.address = address;
        } else {
            throw new IllegalArgumentException("IPv4 address with Integer ABCABCABCABC " +
                    " where ABC must be 0 <= value <= 255");
        }
    }

    /**
     * Returns {@code true} if the address is the vaild IPv4 address.
     * @return {@code true} if the address is the vaild IPv4 address.
     */
    public static boolean isVaild(int address){
        int octets = 0;
        for (int i = 0; i < IP_ADDRESS_LENGTH; ++i){
           octets = (address >> (8*(3-i))) & 0xff;
           if (octets > 255 || octets < 0){
               return false;
           }
        }
        return true;
    }

    public static boolean isVaild(String address) {
        if (address == null) {
            return false;
        }
		String[] parts = address.split("/");
        if (parts.length == 2) {
            int cidrMask = Integer.parseInt(parts[1]);
            if (cidrMask > 32 || cidrMask < 0) {
                return false;
            }
        }

		String cidrPrefix = parts[0].trim();
        //String ipPatern = "^(25[0-5]|2[0-4]\\d|[0-1]?\\d?\\d)(\\.(25[0-5]|2[0-4]\\d|[0-1]?\\d?\\d)){3}$";
		//return cidrPrefix.matches(ipPatern);

        String [] octets = cidrPrefix.split("\\.");
        if (octets.length != IP_ADDRESS_LENGTH) {
            return false;
        }
        for (String oct : octets) {
            int ele = Integer.parseInt(oct);
            if (ele < 0 || ele > 255) {
                return false;
            }
        }
        return true;
    }


    /**
     * Returns a IPv4 Address instance representing the value of the specified {@code String}
     * @param address  the String representation of the IPv4 Address to be parsed.
     * @return a IPv4 Address instance representing the value of the specified {@code String}
     * @throws IllegalArgumentException if the String cannot be parsed as a IPv4 address
     */
    public static IPv4Address valueOf(String address){
        if (address == null) {
            throw new IllegalArgumentException("the byte array of IPv4 address cannot be null");
        }
        String [] octets = address.split("\\.");
        if (octets.length != IP_ADDRESS_LENGTH) {
            throw new IllegalArgumentException("Specified IPv4 address must contain 4 byte array");
        }
        int result = 0;
        for (int i = 0; i < IP_ADDRESS_LENGTH; ++i){
            int oct = Integer.valueOf(octets[i]);
            if (oct >= 0 && oct <= 255) {
                result |= oct << (8*(3-i));
            } else {
                throw new IllegalArgumentException("Octet values in specified" +
                        " IPv4 address must be 0 <= value <= 255");
            }
        }
        return new IPv4Address(result);
    }

    /**
     * Returns a IPv4 Address instance representing the value of the specified {@code byte}
     * @param address  the byte array representation of the IPv4 Address to be parsed.
     * @return a IPv4 Address instance representing the value of the specified {@code byte}
     * @throws IllegalArgumentException if the byte array length isn't equal to 4.
     */
    public static IPv4Address valueOf(byte[] address){
        if (address == null){
            throw new IllegalArgumentException("the byte array of IPv4 address cannot be null");
        }
        if (address.length != IP_ADDRESS_LENGTH){
            throw new IllegalArgumentException("Specified IPv4 address must contain 4 byte array");
        }
        int result = 0;
        for (int i = 0; i < IP_ADDRESS_LENGTH; i++){
            result |= (address[i] & 0xff) << (8*(3-i));
        }
        return new IPv4Address(result);
    }

    /**
     * Returns a IPv4 Address instance representing the value of the specified {@code long}
     * @param address  the long representation of the IPv4 Address to be parsed.
     * @return a IPv4 Address instance representing the value of the specified {@code long}
     * @throws IllegalArgumentException if the long cannot be parsed as a IPv4 address
     */
    public static IPv4Address valueOf(long address){
        if (IPv4Address.isVaild((int)address)){
            return new IPv4Address((int)address);
        } else {
            throw new IllegalArgumentException("IPv4 address with Integer ABCABCABCABC " +
                    " where ABC must be 0 <= value <= 255");
        }
    }


    /**
     * Returns the value of the {@code IPv4Address} as a {@code byte} array.
     * @return a {@code byte} array representing the IPv4 address.
     */
    public byte[] toBytes(){
        return new byte[] {
                (byte)(this.address >>> 24),
                (byte)(this.address >>> 16),
                (byte)(this.address >>> 8),
                (byte)this.address};
    }

    /**
     * Returns the value of the {@code IPv4Address} as a {@code long}.
     * @return a numerical value representing the IPv4 address.
     */
    public long toLong() {
        return (long)this.address;
    }

    public int toInt() {
        return this.address;
    }

    public static String intToStr(int ip) {
        return (ip >> 24 & 255) + "."
				+ ((ip >> 16) & 255) + "."
				+ ((ip >> 8) & 255) + "."
				+ (ip & 255);
    }

    public static int strToInt(String ipAddress) {
        if (ipAddress == null)
            throw new IllegalArgumentException("Specified IPv4 address must" +
                "contain 4 sets of numerical digits separated by periods");
        String[] octets = ipAddress.split("\\.");
        if (octets.length != 4)
            throw new IllegalArgumentException("Specified IPv4 address must" +
                "contain 4 sets of numerical digits separated by periods");

        int result = 0;
        for (int i = 0; i < 4; ++i) {
            int oct = Integer.valueOf(octets[i]);
            if (oct > 255 || oct < 0)
                throw new IllegalArgumentException("Octet values in specified" +
                        " IPv4 address must be 0 <= value <= 255");
            result |=  oct << ((3-i)*8);
        }
        return result;
    }

	/**
	 * parse the given CIDR IP, default mask is 32
	 * @return an array contains the CIDR prefix and mask bits
	 */
	public static int[] parseCIDR(String cidr) {
        if (cidr == null)
            throw new IllegalArgumentException("ip/mask is expected");

		int ret[] = new int[2];
		String[] parts = cidr.split("/");
		String cidrPrefix = parts[0].trim();
		int cidrMaskBits = 32;
		if (parts.length == 2) {
			try {
				cidrMaskBits = Integer.parseInt(parts[1].trim());
			} catch (Exception e) {
				throw new NumberFormatException("CIDR mask bits must be specified as a number(0 ~ 32).");
			}
			if (cidrMaskBits < 0 || cidrMaskBits > 32) {
				throw new NumberFormatException("CIDR mask bits must be 0 <= value <= 32.");
			}
		}
		ret[0] = IPv4Address.strToInt(cidrPrefix);
		ret[1] = cidrMaskBits;

		return ret;
	}

	/**
	 * check whether the CIDR address contains the IP address
	 */
	public static boolean containIP(int cidrPrefix, int cidrMaskBits, int ip) {

		boolean matched = true;
		int bitsToShift = 32 - cidrMaskBits;

		if (bitsToShift > 0) {
			cidrPrefix = cidrPrefix >> bitsToShift;
			ip = ip >> bitsToShift;
			cidrPrefix = cidrPrefix << bitsToShift;
			ip = ip << bitsToShift;
		}

		if (cidrPrefix != ip) {
			matched = false;
		}

		return matched;
	}

	/**
	 * check whether cidr1 is a subnet of (or the same as) cidr2
	 * 
	 */
	public static boolean isSubnet(String cidr1, String cidr2) {

		if (cidr2 == null) {
			return true;
		} else if (cidr1 == null) {
			return false;
		}

		int[] cidr = IPv4Address.parseCIDR(cidr1);
		int cidr1Prefix = cidr[0];
		int cidr1MaskBits = cidr[1];
		cidr = IPv4Address.parseCIDR(cidr2);
		int cidr2Prefix = cidr[0];
		int cidr2MaskBits = cidr[1];

		int bitsToShift_1 = 32 - cidr1MaskBits;
		int bitsToShift_2 = 32 - cidr2MaskBits;

		int offset = (bitsToShift_1 > bitsToShift_2) ? bitsToShift_1
				: bitsToShift_2;

		if (offset > 0) {
			cidr1Prefix = cidr1Prefix >> offset;
			cidr2Prefix = cidr2Prefix >> offset;
			cidr1Prefix = cidr1Prefix << offset;
			cidr2Prefix = cidr2Prefix << offset;
		}

		if (cidr1Prefix == cidr2Prefix) {
			if (cidr1MaskBits >= cidr2MaskBits) {
				return true;
			}
		}
		return false;
	}

    @Override
    public String toString(){
        StringBuffer sip = new StringBuffer();
        int result = 0;
        for (int i = 0; i < IP_ADDRESS_LENGTH; ++i){
            result = (this.address >> (8*(3-i))) & 0xff;
            sip.append(Integer.valueOf(result).toString());
            if (i != 3){
                sip.append(".");
            }
        }
        return sip.toString();
    }

    @Override
    public boolean equals(Object o) {
        if (o == this) {
            return true;
        }

        if (!(o instanceof IPv4Address)) {
            return false;
        }

        IPv4Address other = (IPv4Address)o;
        return this.address == other.address;
    }

    @Override
    public int hashCode(){
        return Integer.valueOf(this.address).hashCode();
    }
}

