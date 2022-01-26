(async function() {
  exitStatus = 0;
  const fs = require('fs').promises;
  const path = require('path');

  // get arguments of process run
  const args = process.argv.slice(2);

  const params = {
      packageFilePath : path.resolve(process.cwd(), 'package.json'),
      outputFilePath : path.resolve(process.cwd(), 'dist/reports/audit-report.json'),
      inputFilePath : path.resolve(process.cwd(), 'dist/reports/audit-report-dependency.json')
  };

  // if args have good size run converting
  if (args.length === 1) {
      const [inputFile] = args;

      const npmSeverityToSonar = new Map([
          ["info", 'INFO'],
          ["low", 'MINOR'],
          ["moderate", 'MINOR'],
          ["high", 'MAJOR'],
          ['critical', 'CRITICAL']
      ]);

      let auditJsonString = '';
      try {
          auditJsonString = await fs.readFile(params.inputFilePath);
          auditJsonString = auditJsonString.toString();

      } catch (error) {
          console.error('input file read failed', error);
          exitStatus = 3;
          process.exit(3);
      }

      const audit = JSON.parse(auditJsonString);
      const issues = [];
      const engineId = `npm-audit-${audit.auditReportVersion}`;
      for (const [packageName, vulnerability] of Object.entries(audit.vulnerabilities)) {
          if (vulnerability.isDirect) {
              const packageJsonFile = (await fs.readFile(params.packageFilePath)).toString();
              const packageNameIndex = packageJsonFile.indexOf(packageName);
              const rows = packageJsonFile.slice(0, packageNameIndex).split('\n');
              const startLine = rows.length;
              const startColumn = rows.at(-1).length;
              const endColumn = startColumn + packageName.length;
          
              issues.push({
                  engineId,
                  ruleId : 'dependency-vulnerability',
                  severity : npmSeverityToSonar.get(vulnerability.severity),
                  type : 'VULNERABILITY',
                  primaryLocation : {
                      message : `The dependency ${packageName} has vulnerability${vulnerability.fixAvailable ? `, fix available in ${vulnerability.fixAvailable.name} version : ${vulnerability.fixAvailable.version}` : ''}`,
                      filePath : params.packageFilePath,
                      textRange : {
                          startLine,
                          startColumn,
                          endColumn
                      }
                  }
              });
          }
      }
      
      issues.push({
          engineId,
          ruleId : 'summary-dependency-vulnerability',
          severity : 'INFO',
          type : 'VULNERABILITY',
          primaryLocation : {
              message : Object.entries(audit.metadata.vulnerabilities).map((entry) => {
                  const [level, quantity] = entry;
                  return `- ${quantity} ${level}`;
              }).join('\n'),
              filePath : params.packageFilePath,
              textRange : {
                  startLine : 1,
                  startColumn : 0
              }
          }
      });
      const output = JSON.stringify({issues}, null, 4);
      console.debug('issues generated', output);
      try {
          await fs.writeFile(params.outputFilePath, output);
      } catch (error) {
          console.error('output file write failed', error);
          exitStatus = 2;
      }
  } else {
      console.error('one argument expected,', args.length, 'found');
      exitStatus = 1;
  }
  process.exit(exitStatus);
})();
