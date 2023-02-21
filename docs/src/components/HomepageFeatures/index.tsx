import React from 'react';
import clsx from 'clsx';
import styles from './styles.module.css';

type FeatureItem = {
  title: string;
  description: JSX.Element;
};

const FeatureList: FeatureItem[] = [
  {
    title: 'Core layer',
    description: (
      <>
        Foundational core layer for new c++ projects.
        farm-ng-core only has a small set of dependencies: libfmt, expected
        (both being targeted for c++ standardization) and Protocol Buffers /
        GRPC.
      </>
    ),
  },
  {
    title: 'Convenient macros',
    description: (
      <>
        FARM_ENUM enables to/from string conversions. FARM_ASSERT_* for
        documenting and enforcing preconditions. FARM_INFO_* log macros with
        run-time and compile time log levels. Convenient macros around Expected
        and more.
      </>
    ),
  },
  {
    title: 'Event logging and more',
    description: (
      <>
        EventLogReader and EventLogWriter to create and playback Protocol
        Buffer-based datasets and some more general purpose c++ utilities.
      </>
    ),
  },
];

function Feature({title, Svg, description}: FeatureItem) {
  return (
    <div className={clsx('col col--4')}>
      <div className="text--center padding-horiz--md">
        <h3>{title}</h3>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures(): JSX.Element {
  return (
    <section className={styles.features}>
      <div className="container">
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}
