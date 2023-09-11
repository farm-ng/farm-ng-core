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
        Foundational core layer for new c++ and python projects.

        Convenient macros such as FARM_ENUM, FARM_ASSERT_* and FARM_UNWRAP.

        Component pipeline for easy parallelization of data processing pipelines.

        farm-ng-core only has a small set of dependencies: libfmt, expected
        (both being targeted for c++ standardization) and Protocol Buffers /
        GRPC.
      </>
    ),
  },
  {
    title: 'sophus2: 2d / 3d Geometry c++ Library',
    description: (
      <>
      Lie Groups / Manifolds commonly used for 2d / 3D geometric problems.
      Image Classes: Image, MutImage, DynImage, MutDynImage and view classes.
      Collection of camera models (pinhole, brown-conrady aka opencv,
      kannala-brandt and orthographic), IMU mode and more. Python bindings
      are available for a subset of the library.
      </>
    ),
  },
  {
    title: 'Multi-language serialization and logging utilities',
    description: (
      <>
        EventLogReader and EventLogWriter to create and playback Protocol
        Buffer-based datasets and some more general purpose c++/python utilities.
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
