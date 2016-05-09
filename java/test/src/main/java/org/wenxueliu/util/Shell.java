/**
 * Inspire by hadoop util shell.java
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.wenxueliu.netty.util;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.InputStream;
import java.io.InterruptedIOException;
import java.nio.charset.Charset;
import java.util.Arrays;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.atomic.AtomicBoolean;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * A base class for running a Shell command.
 *
 * <code>Shell</code> can be used to run shell commands like <code>du</code> or
 * <code>df</code>. It also offers facilities to gate commands by
 * time-intervals.
 */
public abstract class Shell {
  public static final Logger LOG = LoggerFactory.getLogger(Shell.class);

  /**
   * Text to include when there are windows-specific problems.
   * {@value}
   */
  private static final String WINDOWS_PROBLEMS =
      "https://wiki.apache.org/hadoop/WindowsProblems";

  /**
   * Name of the windows utils binary: {@value}.
   */
  static final String WINUTILS_EXE = "winutils.exe";

  /**
   * System property for the Hadoop home directory: {@value}.
   */
  public static final String SYSPROP_HADOOP_HOME_DIR = "hadoop.home.dir";

  /**
   * Environment variable for Hadoop's home dir: {@value}.
   */
  public static final String ENV_HADOOP_HOME = "HADOOP_HOME";

  /**
   * query to see if system is Java 7 or later.
   * Now that Hadoop requires Java 7 or later, this always returns true.
   * @deprecated This call isn't needed any more: please remove uses of it.
   * @return true, always.
   */
  @Deprecated
  public static boolean isJava7OrAbove() {
    return true;
  }

  /**
   * Maximum command line length in Windows
   * KB830473 documents this as 8191
   */
  public static final int WINDOWS_MAX_SHELL_LENGTH = 8191;

  /**
   * mis-spelling of {@link #WINDOWS_MAX_SHELL_LENGTH}.
   * @deprecated use the correctly spelled constant.
   */
  @Deprecated
  public static final int WINDOWS_MAX_SHELL_LENGHT = WINDOWS_MAX_SHELL_LENGTH;

  /**
   * Checks if a given command (String[]) fits in the Windows maximum command
   * line length Note that the input is expected to already include space
   * delimiters, no extra count will be added for delimiters.
   *
   * @param commands command parts, including any space delimiters
   */
  public static void checkWindowsCommandLineLength(String...commands)
      throws IOException {
    int len = 0;
    for (String s: commands) {
      len += s.length();
    }
    if (len > WINDOWS_MAX_SHELL_LENGTH) {
      throw new IOException(String.format(
        "The command line has a length of %d exceeds maximum allowed length" +
            " of %d. Command starts with: %s",
        len, WINDOWS_MAX_SHELL_LENGTH,
        StringUtils.join("", commands).substring(0, 100)));
    }
  }
